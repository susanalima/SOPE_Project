
#include "Structures.h"

Flags* create_Flags(int ignoreCase,int showFileName,int showLinesNumber,int sowNumberOfLines,int patternIsFullWord)
{
	Flags* flags;
	int s;
	s = alloc_Flags(&flags);
	if (s != OK) return NULL;
	flags->ignoreCase = ignoreCase;
	flags->showFileName = showFileName;
	flags->showLinesNumber = showLinesNumber;
	flags->sowNumberOfLines = sowNumberOfLines;
	flags->patternIsFullWord = patternIsFullWord;
	return flags;
}

int alloc_Flags(Flags** flags)
{
	*flags = NULL;
	Flags* f = (Flags*)calloc(sizeof(Flags),1);
	if (f == NULL) return ERROR;
	*flags = f;
	return OK;
}

void free_Flags(Flags** flags)
{
	if (*flags == NULL || flags == NULL) return;
	free(*flags);
	*flags = NULL;
}

//nao esta a ver a ordem pela qual eles aparecem
Flags* createAndSet_Flags(int argc, char* argv[])
{
	int i;
	int ignoreCase =FALSE, showFileName=FALSE,showLinesNumber=FALSE,sowNumberOfLines=FALSE,patternIsFullWord=FALSE;
	Flags* flags;
	if (argc == 3) 
	{
		flags = create_Flags(ignoreCase,showFileName,showLinesNumber,sowNumberOfLines,patternIsFullWord);
		return flags;
	}

	for (i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-i") == 0)
			ignoreCase = TRUE;
		if (strcmp(argv[i], "-l") == 0)
			showFileName = TRUE;
		if (strcmp(argv[i], "-n") == 0)
			showLinesNumber = TRUE;
		if (strcmp(argv[i], "-c") == 0)
			sowNumberOfLines = TRUE;
		if (strcmp(argv[i], "-w") == 0)
			patternIsFullWord = TRUE;
		if (strcmp(argv[i], "-r") == 0)
			return NULL;
	}
	flags = create_Flags(ignoreCase,showFileName,showLinesNumber,sowNumberOfLines,patternIsFullWord);
	return flags;
}


Line* create_Line(char* line, int lineNumber, int size)
{
	Line* Line;
	int s;
	s = alloc_Line(&Line, size);
	if (s != OK) return NULL;
	Line->lineNumber = lineNumber;
	strcpy(Line->line,line);
	return Line;
}


int alloc_Line(Line** line, int size)
{
	*line = NULL;
	Line* L = (Line*)calloc(sizeof(Line),1);
	if (L == NULL) return ERROR;
	*line = L;
	(*line)->line = (char*)malloc(size*sizeof(char));
	if ((*line)->line == NULL) return ERROR;
	return OK;
}


void free_Line(Line** line)
{
	if (*line == NULL || line == NULL) return;
	if ((*line)->line != NULL) free(&(*line)->line);
	free(*line);
	*line = NULL;
}



FileInfo* create_FileInfo(char* filename, Flags* flags)
{
	FileInfo* fileInfo;
	int s;
	s = alloc_FileInfo(&fileInfo,filename);
	if (s != OK) return NULL;
	strcpy(fileInfo->filename,filename);
	fileInfo->flags = flags;
	fileInfo->numberOfLinesWithPattern = 0;
	return fileInfo;
}


int alloc_FileInfo(FileInfo** fileInfo,char* filename)
{
	*fileInfo = NULL;
	FileInfo* fi = (FileInfo*)calloc(sizeof(FileInfo),1);
	if( fi == NULL) return ERROR;
	*fileInfo = fi;
	(*fileInfo)->filename = (char*)malloc(strlen(filename)*sizeof(char));
	if ((*fileInfo)->filename == NULL) return ERROR;
	(*fileInfo)->lines = (Line**)malloc(sizeof(Line*));
	if ((*fileInfo)->lines == NULL) return ERROR;
	return OK;
}


//nao faz free do array de line...
void free_FileInfo(FileInfo** fileInfo)
{
	if (fileInfo == NULL || *fileInfo == NULL) return;
	else
	{
		if ((*fileInfo)->flags != NULL) free_Flags(&(*fileInfo)->flags);
		if ((*fileInfo)->filename != NULL) free((*fileInfo)->filename);
		free((*fileInfo)->lines);
		free(*fileInfo);
		*fileInfo = NULL;

	}
}
