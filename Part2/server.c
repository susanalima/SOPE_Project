#include "Structures.h"
#include "seat.h"
#include "logs.h"

#define SHM_SIZE 1



char SHM_NAME[]="/shm1";
char SEM_NAME[]="/sem1";
char OTHER_SEM_NAME[] = "/sem2";
//numero total de lugares na sala

int writen=FALSE;
int flag = FALSE;
Seat* seats;

int num_seats;


//Aceita e executa os pedidos de reserva de bilhetes que lhe são enviados pelos clientes
//(instâncias declient), dando uma resposta adequada a cada cliente

//server <num_seats> <num_ticket_offices> <open_time>

int verifySeat(int number)
{
  if (number > num_seats)
    return IID;
  if (number > MAX_CLI_SEATS)
    return IID;
  return OK;

}


int verifyRequest(Request* request)
{
  if (request->size < request->num_wanted_seats)
    return IID;
    
  if (request->num_wanted_seats < 1 || request->num_wanted_seats > MAX_CLI_SEATS)
     return MAX;

  if (request->size > num_seats || request->num_wanted_seats > num_seats)
    return NST;
  
  for (int i = 0; i < request->size; i++)
  {
    if (verifySeat(request->pref_seat_list[i]) != OK)
      return IID;
  }
    //outros testes;
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
    while(writen==FALSE){}
    //------------------ler mensagem------------------------------------------
    s1=shm1;
    msg1=*s1;
    writen=FALSE;
    sem_post(tsem);


    Request request=msg1;
    Answer answer;
    char fanswer[20];
    printf("Servidor leu\n");
    printf("%d\n",request.time_out);
    sprintf(fanswer,"/tmp/ans%d", request.pid);

    int count = 0;
    int valid_request = verifyRequest(&request);
    if (valid_request == OK)
    {
      printf("hi1\n");
      answer.seq = malloc(request.num_wanted_seats*sizeof(int));
      printf("hi2\n");
      for(int i = 0; i < request.size; i++)
      {
        sem_wait(book_sem);
        printf("hi3\n");
        if (isSeatFree(seats, request.pref_seat_list[i]))
        {
          printf("hi4\n");
          bookSeat(seats,request.pref_seat_list[i], request.pid);
          answer.seq[i] = request.pref_seat_list[i];
          count++; 
        }
        sem_post(book_sem);
         printf("hi5\n");
        if (count == request.num_wanted_seats)
          break;
      }

      if (count != request.num_wanted_seats)
      {
        sem_wait(book_sem);
        freeAllSeats(seats, answer.seq, count);
          printf("hi6\n");
        sem_post(book_sem);
      }

    }

    answer.valid_request = valid_request;
    answer.num_seats = count; //pode nao ser


    int fd_ans = open(fanswer,O_WRONLY);
    printf("Abriu servidor de resposta\n");
    if(fd_ans == -1) {
      printf("Oops !!! Service is closed !!!\n");
      exit(2);
    }
    write(fd_ans, &answer, sizeof(Answer));
    printf("Escrevey\n");
    close(fd_ans);
    printf("fechey\n");
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
  


  int shmfd;
  Request *shm, *s;
  int fd_req;
  sem_t *sem, *othersem;
  //cria a sala
  seats = malloc(num_room_seats*sizeof(Seat));

  for (int j = 0; j < num_room_seats; j++)
  {
    Seat seat;
    seat.is_free = FALSE;
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
  void *re;
  for (i = 0; i < num_ticket_offices; i++)
  {
    pthread_create(&ticket_offices[i], NULL, func,NULL);
  }

  //------------------Criar alarme---------------------------------------------------
  signal(SIGALRM,timeOut);
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
    if(read(fd_req, &request, sizeof(Request))<=0)continue;
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
  printf("Servidor fechou. Timed out\n");
  exit(0);

}
