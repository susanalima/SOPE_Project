#include "libraries.h"
#include "SearchFunctions.h"

//isto depois tem de sair da main...
int main (int argc, char *argv[]) {

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
