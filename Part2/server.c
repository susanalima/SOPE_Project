#include "Structures.h"
#include "seat.h"
#include "logs.h"

#define SHM_SIZE 1



char SHM_NAME[]="/shm1";
char SEM_NAME[]="/sem1";
//numero total de lugares na sala
int num_seats;
int shmfd;
Request *shm, *s;
int fd_req;
sem_t *sem;
int writen=FALSE;


//Aceita e executa os pedidos de reserva de bilhetes que lhe são enviados pelos clientes
//(instâncias declient), dando uma resposta adequada a cada cliente

//server <num_seats> <num_ticket_offices> <open_time>



void *func(void *arg)
{
  while(1){
    //---------------------Abrir semafáro----------------------------------------------
    sem_t *tsem;
    tsem=sem_open(SEM_NAME,0,0600,0);

    if(tsem==SEM_FAILED)
    {
      perror("Ticket office failure sem_open");
      exit(1);
    }

    //espera que fique disponivel
    sem_wait(tsem);
    //espera que esteja uma mensagem no buffer
    while(writen==FALSE){}

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
    shm1 = (Request*) mmap(0,SHM_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,shmfd,0);
    if(shm1 == MAP_FAILED)
    {
      perror("Ticket office failure in mmap()");
      exit(2);
    }
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

    answer.valid_request = 0;
    answer.num_seats = 3;


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
  pthread_exit(NULL);
}



int verifyRequest(Request* request)
{
  if (request->num_wanted_seats < 1 || request->num_wanted_seats > MAX_CLI_SEATS)
    return MAX;
    //outros testes;
  return OK;
}

void timeOut(int signo)
{
  close(fd_req);

  //destroys the fifo requests
  if(unlink("/tmp/requests")<0)
   printf("Error when destroying FIFO '/tmp/requests'\n");
  else
   printf("FIFO '/tmp/requests' has been destroyed\n");

  sem_close(sem);
  sem_unlink(SEM_NAME);
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
  int num_seats = num_room_seats;
  signal(SIGALRM,timeOut);
  alarm(open_time);
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

  //------------------Criar threads---------------------------------------------------
  pthread_t* ticket_offices = malloc(num_ticket_offices * sizeof(pthread_t));

  int i;
  void *re;
  for (i = 0; i < num_ticket_offices; i++)
  {
    pthread_create(&ticket_offices[i], NULL, func,NULL);
  }

  //-------------------creates the fifo requests------------------------------------
  if (mkfifo("/tmp/requests",0660)<0)
  if(errno==EEXIST) printf("FIFO already exists\n");
  else
    printf("Can't create FIFO\n");
  else
    printf("FIFO sucessfully created\n");

  //------------------------------reads a request---------------------------------
  fd_req = open("/tmp/requests",O_RDONLY);

  while(1){
    Request request;
    Answer answer;
    if(read(fd_req, &request, sizeof(Request))<=0)continue;
    while(writen==TRUE){}

    s = shm;
    *s=request;
    writen = TRUE;


  }



}
