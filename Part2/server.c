#include "Structures.h"



int num_seats;
int fd_req;


//Aceita e executa os pedidos de reserva de bilhetes que lhe são enviados pelos clientes
//(instâncias declient), dando uma resposta adequada a cada cliente

//server <num_seats> <num_ticket_offices> <open_time>

//numero total de lugares na sala

void *func(void *arg)
{

}

//todo
int write_to_slog(pid_t pid, Answer* answer)
{
  return OK;
}

//NAO PERCEBI
//Cada  uma destas funções, após a  operação de  leitura/escrita do conteúdo de
//seats, deve  invocar dentro na  secção  crítica  respetiva uma function-like  macro, DELAY(),
//definida  usando  a  diretiva: #define DELAY()

int isSeatFree(Seat *seats, int seatNum)
{
  int i;
  for (i = 0; i < num_seats; i++)
  {
    if (seats[i].number == seatNum)
    {
      return (seats[i].is_free);
    }
  }
  return ERROR;
}

void bookSeat(Seat *seats, int seatNum, int clientId)
{
  int i;
  for (i = 0; i < num_seats; i++)
  {
    if (seats[i].number == seatNum)
    {
      seats[i].clientId = clientId;
      seats[i].is_free = FALSE;
      return;
    }
  }
}

void freeSeat(Seat *seats, int seatNum)
{
  int i;
  for (i = 0; i < num_seats; i++)
  {
    if (seats[i].number == seatNum)
    {
      seats[i].clientId = 0;
      seats[i].is_free = TRUE;
      return;
    }
  }
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

//free(ticket_offices);
  exit(0);
}

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    perror("ERROR");
    exit(1);
  }
  int num_room_seats = atoi(argv[1]);
  int num_ticket_offices = atoi(argv[2]);
  int open_time = atoi(argv[3]);
  int num_seats = num_room_seats;

  //pthread_create(&threads[3], NULL, divv, (void*)&td );

  pthread_t* ticket_offices = malloc(num_ticket_offices * sizeof(pthread_t));

  int i;
  for (i = 0; i < num_ticket_offices; i++)
  {
    pthread_create(&ticket_offices[i], NULL, func);
  }

  //creates the fifo requests
  if (mkfifo("/tmp/requests",0660)<0)
  if(errno==EEXIST) printf("FIFO already exists\n");
  else
    printf("Can't create FIFO\n");
  else
    printf("FIFO sucessfully created\n");

  //reads a request
  signal(SIGALRM,timeOut);
  alarm(open_time);
  fd_req = open("/tmp/requests",O_RDONLY);

  while(1){
    Request request;
    Answer answer;
    char fanswer[20];
   if(read(fd_req, &request, sizeof(Request))<=0)continue;
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



}
