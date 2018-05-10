#include "Structures.h"
#include "seat.h"
#include "logs.h"


//char OUT_SEM_NAME[] = "/sem3";

 pid_t pid;

void clientTimeOut(int signo)
{
	printf("time out\n");
	Answer answer;
	answer.valid_request = OUT;
	write_to_clog(pid,&answer);
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

 	pid = getpid();
	request.pid = pid;
  char fanswer[20];
  sprintf(fanswer,"/tmp/ans%d", pid);


	//request.pref_seat_list = malloc(sizeof(int));

  int tmp_seats[MAX_CLI_SEATS];
	int count = 1;
	char* token;
	token = strtok(argv[3]," ");
	//request.pref_seat_list[0] = atoi(token);
  tmp_seats[0] = atoi(token);
	while(token != NULL)
	{
		count++;
		//request.pref_seat_list = realloc(request.pref_seat_list,count*sizeof(int));
		token = strtok(NULL, " ");
		if(token == NULL)
			break;
		//request.pref_seat_list[count-1] = atoi(token);
    tmp_seats[count-1] = atoi(token);
	}


	request.size = count-1; //acho que e isto
  memcpy(request.pref_seat_list,tmp_seats,request.size*sizeof(int));
	printf("%d v\n\n",request.size);

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

	signal(SIGALRM,clientTimeOut);
	alarm(request.time_out);

	write(fd_req, &request, sizeof(request));

//	write(fd_req, request.pref_seat_list, sizeof(int)*request.size+1);

	close(fd_req);





		//opens the answer fifo
	int fd_ans = open(fanswer, O_RDONLY);
	read(fd_ans, &answer, sizeof(Answer));
	close(fd_ans);

  //writes to clog
	write_to_clog(request.pid, &answer);

  //writes to cbook
  if(answer.valid_request == OK)
    write_to_cbook(answer.seq, answer.num_seats);

	//printf("%d\n", answer.num_seats);
//	printf("%d\n", answer.valid_request);

  for (int k = 0; k < answer.num_seats; k++)
    printf("%d\n", answer.seq[k]);


   //destroys the fifo ansXXXXX
  if(unlink(fanswer)<0)
    printf("Error when destroying FIFO \n");
  else
    printf("FIFO has been destroyed\n");



  exit(0);
}
