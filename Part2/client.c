
#include "Structures.h"


int write_to_clog(pid_t pid, Answer* answer)
{
	int fd;
	char to_write[1000];
	//FALTA A CENA DA PRECISAO
	switch(answer->valid_request)
	{
		case MAX:
			sprintf(to_write,"%d MAX", pid);
			break;
		case NST:
			sprintf(to_write,"%d NST", pid);
			break;
		case IID:
			sprintf(to_write,"%d IID", pid);
			break;
		case ERR:
			sprintf(to_write,"%d ERR", pid);
			break;
		case NAV:
			sprintf(to_write,"%d NAV", pid);
			break;
		case FUL:
			sprintf(to_write,"%d FUL", pid);
			break;
		case OK:
			sprintf(to_write,"%d OK", pid); // NAO E ISTO QUE E PARA ESCREVER
			break;
	}

	fd = open("clog.txt",O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		return ERROR;
	}
	write(fd, to_write, strlen(to_write));
	//write(fd, "\n",2);
	close(fd);
	return OK;
}

/*
	char* token;
	token = strtok(argv[3]," ");
	while(token != NULL)
	{
		printf("%d\n",atoi(token));
		token = strtok(NULL, " ");
	}

*/

void clientTimeOut(int signo)
{
	printf("time out\n");
	exit(1);
}

//client <time_out> <num_wanted_seats> <pref_seat_list>
int main(int argc, char *argv[])
{
  if (argc < 4)
	{
		perror("ERROR");
		exit(1);
	}

	Request request;
	Answer answer;
  request.time_out = atoi(argv[1]);
  request.num_wanted_seats = atoi(argv[2]);
  request.pref_seat_list = malloc(sizeof(*argv[3])+1);
  memset(request.pref_seat_list, '\n', sizeof(*argv[3]));
  strcpy(request.pref_seat_list, argv[3]);
  pid_t pid = getpid();
	request.pid = pid;
  char fanswer[20];
  sprintf(fanswer,"/tmp/ans%d", pid);

  //creates the fifo ansXXXXX
  if (mkfifo(fanswer,0660)<0)
  if(errno==EEXIST) printf("FIFO already exists\n");
  else
   printf("Can't create FIFO\n");
  else
   printf("FIFO sucessfully created\n");


	 //opens the requests fifo
	int fd_req = open("/tmp/requests",O_WRONLY);
	if(fd_req == -1) {
	   printf("Oops !!! Service is closed !!!\n");
	   exit(2);
	}
	write(fd_req, &request, sizeof(Request));
	close(fd_req);

	//FALTA A CENA DO TIME OUT

	signal(SIGALRM,clientTimeOut);
	alarm(request.time_out);


		//opens the answer fifo
		int fd_ans = open(fanswer, O_RDONLY);
		read(fd_ans, &answer, sizeof(Answer));
		close(fd_ans);
		printf("%d\n",answer.num_seats);
		write_to_clog(request.pid, &answer);

/*
	int fd_ans = open(fanswer, O_RDONLY);
	read(fd_ans, &answer, sizeof(Answer));
	close(fd_ans);

	write_to_clog(request.pid, &answer);
*/

   //destroys the fifo ansXXXXX
  if(unlink(fanswer)<0)
    printf("Error when destroying FIFO \n");
  else
    printf("FIFO has been destroyed\n");
  exit(0);
}
