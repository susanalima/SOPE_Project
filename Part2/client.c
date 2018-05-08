#include "Structures.h"
#include "seat.h"
#include "logs.h"


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
  //request.pref_seat_list = malloc(sizeof(*argv[3])+1);
  //memset(request.pref_seat_list, '\n', sizeof(*argv[3]));
  //strcpy(request.pref_seat_list, argv[3]);
  pid_t pid = getpid();
	request.pid = pid;
  char fanswer[20];
  sprintf(fanswer,"/tmp/ans%d", pid);


	request.pref_seat_list = malloc(sizeof(int));
	int count = 0;
	char* token;
	token = strtok(argv[3]," ");
	request.pref_seat_list[0] = atoi(token);
	while(token != NULL)
	{
		count++;
		request.pref_seat_list = realloc(request.pref_seat_list,count*sizeof(int));
		request.pref_seat_list[count] = atoi(token);
		token = strtok(NULL, " ");
	}


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

	signal(SIGALRM,clientTimeOut);
	alarm(request.time_out);


		//opens the answer fifo
	int fd_ans = open(fanswer, O_RDONLY);
	read(fd_ans, &answer, sizeof(Answer));
	close(fd_ans);
	printf("%d\n",answer.num_seats);
	write_to_clog(request.pid, &answer);


   //destroys the fifo ansXXXXX
  if(unlink(fanswer)<0)
    printf("Error when destroying FIFO \n");
  else
    printf("FIFO has been destroyed\n");
  exit(0);
}
