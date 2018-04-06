
#include "SearchFunctions.h"


//nao devia estar aqui
char* strlwr(char *str)
{
  char *character = str;
  while (*character) {
     *character = tolower(*character);
      character++;
  }
  return str;
}

//nao devia estar aqui
int isDirectory(char* path)
{
  struct stat sbuf;
  if(stat(path,&sbuf) != OK)
    return 0;
  return S_ISDIR(sbuf.st_mode); //returns non zero if the file is a directory
}


int LineSearch(Flags* flags, char* pattern, char* line)
{
	char patternCopy[strlen(pattern+1)];
	memset(patternCopy,'\n',sizeof(*pattern));
	strcpy(patternCopy,pattern);

	char lineCopy[strlen(line)+1];
	memset(lineCopy,'\n',sizeof(*line));
	strcpy(lineCopy,line);

	if (flags->ignoreCase )
	{
		strlwr(lineCopy);
		strlwr(patternCopy);
	}

	if (!flags->patternIsFullWord)
	{
		if(strstr(lineCopy,patternCopy)) return TRUE;
		else return FALSE;
	}
	else
		return WordSearch(patternCopy,lineCopy);
}


int WordSearch(char* pattern, char* line)
{
	char* token;
	token = strtok(line," \n,.!?:-;");
	while(token != NULL)
	{
		if (strcmp(token,pattern) == 0)
				return TRUE;
		token = strtok(NULL, " \n,.!?:-;");
	}
	return FALSE;
}


int FileSearch(FileInfo* fileInfo, char* pattern)
{
	int lineNumber = 0;
	int counter = 0;
	int numberOfLinesWithPattern = 0;

	FILE* file;
	file = fopen(fileInfo->filename,"r");
	if(file == NULL) {
	      perror("Error opening file");
	      return ERROR;
	}

  char* str;
  size_t nbyte;
  ssize_t size;
	while((size=getline(&str,&nbyte,file))!=-1)
	{
		lineNumber++;
    if (LineSearch(fileInfo->flags,pattern,str))
		{
			numberOfLinesWithPattern++;
			fileInfo->lines = realloc(fileInfo->lines, numberOfLinesWithPattern*sizeof(Line));
      Line *line=create_Line(str,lineNumber,size+1);
      if(line==NULL)
        return ERROR;
			fileInfo->lines[counter] = *line;
			counter++;
		}
	}
	fclose(file);
	fileInfo->numberOfLinesWithPattern = counter;
	return OK;
}

int DirectorySearch(Flags* flags, char*pattern, char* path)
{
  DIR *dir;
  s

}


void PrintFileInfo(FileInfo* fileInfo)
{
	if (fileInfo->numberOfLinesWithPattern == 0)
	{
		printf("The given pattern was not found\n");
		return;
	}
	if (fileInfo->flags->showFileName)
	{
		printf("%s\n", fileInfo->filename);
		return;
	}

	if(fileInfo->flags->sowNumberOfLines)
	{
		printf("%d\n",fileInfo->numberOfLinesWithPattern );
		return;
	}

	int i;
	for (i = 0; i < fileInfo->numberOfLinesWithPattern; i++)
	{
		if (fileInfo->flags->showLinesNumber)
			printf("%d : ", fileInfo->lines[i].lineNumber);
		printf("%s", fileInfo->lines[i].line);
	}
  printf("\n");
}
