#include "Structures.h"
#include "seat.h"
#include "logs.h"

#define SHM_SIZE 1

char SHM_NAME[]="/shm1";
char SEM_NAME[]="/sem1";
char OTHER_SEM_NAME[] = "/sem2";
char OUT_SEM_NAME[] = "/sem3";


int writen=FALSE;
int flag = FALSE;
Seat* seats;
int full_room = FALSE;
int num_seats;


//Aceita e executa os pedidos de reserva de bilhetes que lhe são enviados pelos clientes
//(instâncias declient), dando uma resposta adequada a cada cliente

//server <num_seats> <num_ticket_offices> <open_time>

int verifySeat(int number)
{
  if (number > num_seats || number > MAX_CLI_SEATS)
    return IID;
    //erro no atoi
  if (number == 0)
    return ERR;
  if (number < 0)
	  return IID;
  return OK;
}

//verifica se a sala esta cheia
int is_room_full()
{
  if (full_room == TRUE)
    return TRUE;
  for(int i = 0; i < num_seats; i++)
  {
    if (seats[i].is_free == TRUE)
      return FALSE;
  }
  return TRUE;
}

//valida o pedido
int verifyRequest(Request* request)
{
  full_room = is_room_full();
  if (full_room == TRUE)
    return FUL;

  if (request->size < request->num_wanted_seats)
    return IID;

  if (request->num_wanted_seats < 1 || request->num_wanted_seats > MAX_CLI_SEATS)
     return MAX;

  if (request->size > num_seats || request->num_wanted_seats > num_seats)
    return NST;

  int validation;
  for (int i = 0; i < request->size; i++)
  {
    validation = verifySeat(request->pref_seat_list[i]);
    if (validation != OK)
      return validation;
  }

  return OK;
}



void *func(void *arg)
{

  //---------------------Abrir semafáros----------------------------------------------
  sem_t *tsem, *book_sem;
  tsem=sem_open(SEM_NAME,0,0600,0);

  if(tsem==SEM_FAILED)
  {
    perror("Ticket office failure sem_open");
    exit(1);
  }

  book_sem=sem_open(OTHER_SEM_NAME,0,0600,0);

  if(book_sem==SEM_FAILED)
  {
    perror("Ticket office failure sem_open");
    exit(1);
  }


  //---------------------open the shared memory region ------------------------------------
  int shmfd1;
  Request *shm1,*s1,msg1;
  shmfd1 = shm_open(SHM_NAME,O_RDWR,0600);
  if(shmfd1<0)
  {
    perror("Ticket office failure in shm_open()");
    exit(1);
  }
  //---------------------attach this region to virtual memory-------------------------
  shm1 = (Request*) mmap(0,SHM_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,shmfd1,0);
  if(shm1 == MAP_FAILED)
  {
    perror("Ticket office failure in mmap()");
    exit(2);
  }


  while(flag == FALSE || writen == FALSE){

    //espera que fique disponivel
    sem_wait(tsem);
    //espera que esteja uma mensagem no buffer
    while(writen==FALSE && flag == FALSE){}
    if(flag==TRUE)
    {
      sem_post(tsem);
      break;
    }
    //------------------ler mensagem------------------------------------------
    s1=shm1;
    msg1=*s1;
    writen=FALSE;
    sem_post(tsem);


    Request request=msg1;
    Answer answer;
    char fanswer[20];
    sprintf(fanswer,"/tmp/ans%d", request.pid);

    int count_invalido = request.size;
    int count_valido = 0;
    int valid_request = verifyRequest(&request);

    if (valid_request == OK)
    {
      //answer.seq = malloc(request.num_wanted_seats*sizeof(int));
      for(int i = 0; i < request.size; i++)
      {
        printf("sem_wait %lu\n",pthread_self());
        sem_wait(book_sem);
        printf("sem_wait finish %lu\n",pthread_self());
        if (isSeatFree(seats, request.pref_seat_list[i]))
        {
          bookSeat(seats,request.pref_seat_list[i], request.pid);
          answer.seq[i] = request.pref_seat_list[i];
          count_valido++;
        }
        else{
          count_invalido--;
        }
        printf("sem_post %lu\n",pthread_self());
        sem_post(book_sem);
        printf("sem_post finish %lu\n",pthread_self());
        if (count_invalido < request.num_wanted_seats || count_valido>=request.num_wanted_seats)
          break;
      }

      if (count_invalido < request.num_wanted_seats)
      {
        sem_wait(book_sem);
        freeAllSeats(seats, answer.seq,count_valido);
        count_valido = 0;
        valid_request = NAV;
        sem_post(book_sem);
      }

    }
    answer.valid_request = valid_request;
    answer.num_seats = count_valido;



    int fd_ans = open(fanswer,O_WRONLY | O_NONBLOCK );
    if(fd_ans == -1) {
      printf("Oops !!! Cliente fifo doesn't exist!!!\n");
      sem_wait(book_sem);
      freeAllSeats(seats, answer.seq,answer.num_seats);
      sem_post(book_sem);
    }
    else
    {
      write(fd_ans, &answer, sizeof(Answer));
      close(fd_ans);
    }

    write_to_slog(request.pid, *(int*)arg, &request, &answer);
  }


  //----------------------------Fechar semafaro-----------------------------------------
  sem_close(tsem);

  sem_close(book_sem);
  //----------------------------Fechar memoria partilhada-------------------------------
  if(munmap(shm1,SHM_SIZE) < 0)
  {
    perror("Ticket office failure munmap");
    exit(5);
  }

  pthread_exit(NULL);
}





void timeOut(int signo)
{
  flag = TRUE;
}

int main(int argc, char *argv[])
{
  //------------------Verificar se os parametros estão corretos----------------------------------------------
  if (argc < 4)
  {
    perror("ERROR");
    exit(1);
  }
  //------------------Variaveis do programa server-------------------------------------------------
  int num_room_seats = atoi(argv[1]);
  int num_ticket_offices = atoi(argv[2]);
  int open_time = atoi(argv[3]);
  num_seats = num_room_seats;

  //deletes existing files
  remove("clog.txt");
  remove("cbook.txt");
  remove("slog.txt");
  remove("sbook.txt");

  open_slog();

  int shmfd;
  Request *shm, *s;
  int fd_req;
  sem_t *sem, *othersem;
  //cria a sala
  seats = malloc(num_room_seats*sizeof(Seat));

  for (int j = 0; j < num_room_seats; j++)
  {
    Seat seat;
    seat.is_free = TRUE;
    seat.clientId = -1;
    seat.number = j+1;
    seats[j] = seat;
  }

  //----------------------Memoria Partilhada-------------------------------------------------
  shmfd = shm_open(SHM_NAME,O_CREAT|O_RDWR,0600);
  if(shmfd<0)
  {
    perror("Can't create shared memory\n");
    exit(3);
  }
  if(ftruncate(shmfd,SHM_SIZE) < 0)
  {
    perror("WRITER failure in ftruncate()\n");
    exit(2);
  }
  shm = (Request*) mmap(0,SHM_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,shmfd,0);
  if(shm == MAP_FAILED)
  {
    perror("WRITER failure in mmap()\n");
    exit(3);
  }

  //-----------------------Semafaro---------------------------------------------------------

  sem= sem_open(SEM_NAME,O_CREAT,0600,0);
  if(sem == SEM_FAILED)
  {
    perror("Server failed to create semafaro\n");
    exit(4);
  }
  sem_post(sem);
  othersem= sem_open(OTHER_SEM_NAME,O_CREAT,0600,0);
  if(othersem == SEM_FAILED)
  {
    perror("Server failed to create semafaro\n");
    exit(4);
  }
  sem_post(othersem);

  //------------------Criar threads---------------------------------------------------
  pthread_t* ticket_offices = malloc(num_ticket_offices * sizeof(pthread_t));

  int i;
  int* t_arg = malloc(num_ticket_offices*sizeof(int));
  for (i = 0; i < num_ticket_offices; i++)
  {
    t_arg[i] = i+1;
    pthread_create(&ticket_offices[i], NULL, func,(void*)&t_arg[i]);
    write_open_ticketOffice(t_arg[i]);
  }
  //------------------Criar alarme---------------------------------------------------

  struct sigaction action;
  action.sa_flags = 0;
  action.sa_handler = timeOut;
  if (sigaction(SIGALRM, &action,NULL) < 0)
  {
    fprintf(stderr,"Unable to install SIGALRM handler\n");
    exit(1);
  }
  //signal(SIGALRM,timeOut);
  alarm(open_time);


  //-------------------creates the fifo requests------------------------------------
  if (mkfifo("/tmp/requests",0660)<0)
  if(errno==EEXIST) printf("FIFO already exists\n");
  else
    printf("Can't create FIFO\n");
  else
    printf("FIFO sucessfully created\n");

  //------------------------------reads a request---------------------------------
  fd_req = open("/tmp/requests",O_RDONLY);

  while(flag == FALSE){
    Request request;
    //request.pref_seat_list = malloc(4*sizeof(int));
    if(read(fd_req, &request, sizeof(request))<=0)continue;

    while(writen==TRUE){}

    s = shm;
    *s=request;
    writen = TRUE;

  }

  close(fd_req);

  //destroys the fifo requests
  if(unlink("/tmp/requests")<0)
   printf("Error when destroying FIFO '/tmp/requests'\n");
  else
   printf("FIFO '/tmp/requests' has been destroyed\n");


  sem_close(sem);
  sem_unlink(SEM_NAME);

  sem_close(othersem);
  sem_unlink(OTHER_SEM_NAME);

  if(munmap(shm,SHM_SIZE)<0)
  {
    perror("Server failure munmap\n");
    exit(5);
  }
  if(shm_unlink(SHM_NAME) < 0)
  {
    perror("Server failure shm_unlink\n");
    exit(5);
  }

  printf("Server TimeOut. Waiting for ticket offices to close\n");
  for (i = 0; i < num_ticket_offices; i++)
  {
    pthread_join(ticket_offices[i],NULL);
    write_close_ticketOffice(t_arg[i]);
  }

  free(ticket_offices);
  free(t_arg);

  write_close_server();
  close_slog();

  //writes to sbook
  write_to_sbook(seats, num_room_seats);

  exit(0);

}
