#include "libraries.h"
#include "SearchFunctions.h"
static pid_t pai;
int fd[2];
void ler(int signo)
{
	char a[1];
	char *buffer;
	size_t t=300;
	char c='X';
	if(getpid()==pai)
	{
		close(fd[READ]);
		//printf("\n\n\n\nAre you sure you want to terminate the program? (Y/N)");
		//fgets(buffer,sizeof(buffer),stdin);
		//while ((c = getchar()) != '\n' && c != ' ') { }
		getline(&buffer,&t,stdin);
		/*while(c!='Y' && c!='y' && c!='N' && c!='n')
		{
			//c=getchar();
			//scanf(" %c",&c);
			//getline(&buffer,&t,stdin);
			//fgets(buffer,sizeof(buffer),stdin);
			printf("PAI:|%s|\n",buffer);
		}*/
		printf("PAI:|%s|\n",buffer);
		if(strcmp(buffer,"Y"))
		{
			write(fd[WRITE],buffer,1);
		}
		else
		{
			strcpy(a,"N");
			write(fd[WRITE],a,1);
			close(fd[WRITE]);
			exit(0);
		}
	}
	else
	{
		close(fd[WRITE]);
		read(fd[READ],a,1);
		printf("Filho:%s\n",a);
		if(strcmp(a,"N"))
		{
			close(fd[READ]);
			exit(0);
		}
	}
	exit(0);
}
//isto depois tem de sair da main...
int main (int argc, char *argv[]) {
	pai=getpid();
	//pipe(fd);
	pid_t wpid;
	int status;
	Flags flags={FALSE,FALSE,FALSE,FALSE,FALSE};
	char* pattern;
	struct sigaction action;
	action.sa_flags = 0;
	action.sa_handler = ler;
	if (sigaction(SIGINT,&action,NULL) < 0)
	{
		fprintf(stderr,"Unable to install SIGINT handler\n");
		exit(1);
	}
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
		while((wpid = wait(&status)) > 0);
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
