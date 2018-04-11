#include "Structures.h"

//nao esta a ver a ordem pela qual eles aparecem
int set_Flags(int argc, char* argv[],Flags *flags)
{
	if (argc == 3)
	{
		return 0;
	}
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-i") == 0)
			flags->ignoreCase = TRUE;
		if (strcmp(argv[i], "-l") == 0)
			flags->showFileName = TRUE;
		if (strcmp(argv[i], "-n") == 0)
			flags->showLinesNumber = TRUE;
		if (strcmp(argv[i], "-c") == 0)
			flags->sowNumberOfLines = TRUE;
		if (strcmp(argv[i], "-w") == 0)
			flags->patternIsFullWord = TRUE;
		if (strcmp(argv[i], "-r") == 0)
			{
				flags->isdirectory = TRUE;
				return 1;
			}
	}
	return 0;
}


Line * create_Line(char* line, int lineNumber, int size)
{
	Line *linha=malloc(sizeof(Line)*1);
	if(linha==NULL)
		return NULL;
	linha->lineNumber=lineNumber;
	linha->line=malloc(sizeof(char)*size);
	if(linha->line==NULL)
		return NULL;
	strcpy(linha->line,line);
	return linha;
}
