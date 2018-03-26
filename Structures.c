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
			return 1;
	}
	return 0;
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
		//if ((*fileInfo)->flags != NULL) free_Flags(&(*fileInfo)->flags);
		if ((*fileInfo)->filename != NULL) free((*fileInfo)->filename);
		free((*fileInfo)->lines);
		free(*fileInfo);
		*fileInfo = NULL;

	}
}
