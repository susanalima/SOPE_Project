#include "libraries.h"
#include "SearchFunctions.h"
#include "signal.h"
static int desc ;
static long int first_time;
void closeLogFile()
{
	close(desc);
}
void writeLogFile(pid_t pid,char* string)
{
	struct timespec requestEnd;
	clock_gettime(CLOCK_REALTIME, &requestEnd);
	long int time_now=requestEnd.tv_nsec;
	char to_write[1000];
	sprintf(to_write,"%.2lf -%8d -%s",(time_now-first_time)/1000000.0,pid,string);
	write(desc,to_write,strlen(to_write));
	write(desc,"\n",1);
}
int main (int argc, char *argv[]) {
	const char* s = getenv("LOGFILENAME");
	desc = open(s,O_CREAT|O_TRUNC|O_WRONLY|O_SYNC,0600);
	struct timespec requestStart;
	clock_gettime(CLOCK_REALTIME, &requestStart);
	first_time=requestStart.tv_nsec;
	atexit(closeLogFile);
	char send2[100];
	strcpy(send2,"COMANDO ");
	for(int i=0;i<argc;i++)
	{
		strcat(send2,argv[i]);
		strcat(send2," ");
	}
  writeLogFile(getpid(),send2);
	if(s==NULL)
	{
		printf("No file name was given to LOGFILENAME\n");
		exit(1);
	}
	processSignal();
	pid_t wpid;
	int status;
	Flags flags={FALSE,FALSE,FALSE,FALSE,FALSE};
	char* pattern;
	if (argc < 3)
	{
		perror("ERROR");
		exit(1);
	}
	//verificar os argumentos passados ao chamar
	pattern = argv[argc-2];
	set_Flags(argc,argv,&flags);

	FileInfo fileInfo;
	fileInfo.filename=argv[argc-1];
	fileInfo.flags=&flags;
	if((fileInfo.lines=malloc(sizeof(*fileInfo.lines)))==NULL)
	{
		printf("Erro na alocação de memoria\n");
		exit(1);
	}
	if (flags.isdirectory)
	{
		DirectorySearch(&flags, pattern, argv[argc-1]);
		while((wpid = waitpid(0,&status,0)) > 0)
		{
		}
	}
	else
	{
		int s = FileSearch(&fileInfo,pattern);
		if (s == ERROR)
			exit(2);
		PrintFileInfo(&fileInfo);
	}

	return 0;
}
