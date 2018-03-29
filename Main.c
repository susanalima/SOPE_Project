#include "libraries.h"
#include "SearchFunctions.h"

//isto depois tem de sair da main...
int main (int argc, char *argv[]) {


	Flags flags={FALSE,FALSE,FALSE,FALSE,FALSE};
	char* pattern;

	if (argc < 3)
	{
		perror("ERROR");
		exit(1);
	}
	//verificar os argumentos passados ao chamar
	pattern = argv[argc-2];
	if (set_Flags(argc,argv,&flags) == 1) //-r
	{
		printf("work in progress...\n");
		return 0;
	}
	//printf("%d %d %d %d %d",flags.ignoreCase,flags.showFileName,flags.showLinesNumber,flags.sowNumberOfLines,flags.patternIsFullWord);
	FileInfo fileInfo;
	fileInfo.filename=argv[argc-1];
	fileInfo.flags=&flags;
	if((fileInfo.lines=malloc(sizeof(*fileInfo.lines)))==NULL)
		printf("Erro na alocação de memoria\n");

	int s = FileSearch(&fileInfo,pattern);
	if (s == ERROR)
		exit(2);
	PrintFileInfo(&fileInfo);
	return 0;
}
