#include "libraries.h"
#include "SearchFunctions.h"
static pid_t pai;
int fd[2];
void ler(int signo)
{
	char c='X';
	if(getpid()==pai)
	{
		close(fd[READ]);
		printf("\n\n\n\nAre you sure you want to terminate the program? (Y/N)");
		while(c!='Y' && c!='y' && c!='N' && c!='n')
		{
			scanf("%c",&c);
			printf("PAI:|%c|\n",c);
		}
		if(c == 'Y')
		{
			write(fd[WRITE],&c,1);
		}
		else
		{
			write(fd[WRITE],&c,1);
			close(fd[WRITE]);
			exit(0);
		}
	}
	else
	{
		close(fd[WRITE]);
		read(fd[READ],&c,1);
		printf("Filho:%c\n",c);
		if(c=='N')
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
	pipe(fd);
	pid_t wpid;
	int status;
	Flags flags={FALSE,FALSE,FALSE,FALSE,FALSE};
	char* pattern;
	if (signal(SIGINT,ler) < 0)
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
