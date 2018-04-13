#include "SearchFunctions.h"
#include "signal.h"


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
  DIR *dirp;
 struct dirent *direntp;
 struct stat stat_buf;
 int pid;
 char cwd[1024];
 if ((dirp = opendir(path)) == NULL)
  return ERROR;
 while ((direntp = readdir(dirp)) != NULL)
 {
   char direc_name[1025]="";
   strcpy(direc_name,path);
   strcat(direc_name,"/");
   strcat(direc_name,direntp->d_name);
  if (stat(direc_name, &stat_buf)==-1)   // testar com stat()
  {
    printf("error here\n");
    return ERROR;
  }
  if (S_ISREG(stat_buf.st_mode))
  {
    FileInfo fileInfo;
  	fileInfo.filename=direc_name;
  	fileInfo.flags=flags;
  	if((fileInfo.lines=malloc(sizeof(*fileInfo.lines)))==NULL)
    {
      printf("Erro na alocação de memoria\n");
      return ERROR;
    }

  	int s = FileSearch(&fileInfo,pattern);
  	if (s == ERROR)
    {
      printf("erro 2\n");
  		exit(2);
    }
  	PrintFileInfo(&fileInfo);
  }
  else if (S_ISDIR(stat_buf.st_mode))
  {
    if (direntp->d_name[0]!='.' && strcmp(direntp->d_name,"..") != 0 )
    {
      pid = fork();
      if (pid == -1) return ERROR;
      if (pid > 0)
      {
        //wait(NULL);
      }
      else
      {
        processSignalChild();
        if (getcwd(cwd, sizeof(cwd)) == NULL)
          return ERROR;
        DirectorySearch(flags,pattern,direc_name);
        exit(0);
      }
    }

  }
 }

 closedir(dirp);
 wait(NULL);
 return OK;

}


void PrintFileInfo(FileInfo* fileInfo)
{
  if (!fileInfo->flags->isdirectory)
  {
  	if (fileInfo->numberOfLinesWithPattern == 0)
  	{
  		printf("The given pattern was not found\n");
  		return;
  	}
  }
	if (fileInfo->flags->showFileName)
	{
		printf("%s\n", fileInfo->filename);
    //write(STDOUT_FILENO, fileInfo->filename,strlen( fileInfo->filename));
    //write(STDOUT_FILENO,"\n",1);
		return;
	}

	if(fileInfo->flags->sowNumberOfLines)
	{
    if (fileInfo->flags->isdirectory)
    {
      printf("%s: ", fileInfo->filename);
      //write(STDOUT_FILENO, fileInfo->filename,strlen( fileInfo->filename));
      //write(STDOUT_FILENO,": ",2);
    }
		printf("%d\n",fileInfo->numberOfLinesWithPattern );
    //char str[12];
    //sprintf(str, "%d", fileInfo->numberOfLinesWithPattern);
    //write(STDOUT_FILENO, str,strlen( str));
    //write(STDOUT_FILENO,"\n",1);
		return;
	}

	int i;
	for (i = 0; i < fileInfo->numberOfLinesWithPattern; i++)
	{
    if (fileInfo->flags->isdirectory)
    {
      printf("%s: ", fileInfo->filename);
      //write(STDOUT_FILENO, fileInfo->filename,strlen( fileInfo->filename));
      //write(STDOUT_FILENO,": ",2);
    }
		if (fileInfo->flags->showLinesNumber)
    {
			printf("%d : ", fileInfo->lines[i].lineNumber);
      //char str[12];
      //sprintf(str, "%d", fileInfo->lines[i].lineNumber);
      //write(STDOUT_FILENO, str,strlen(str));
      //write(STDOUT_FILENO,": ",2);
    }
		printf("%s", fileInfo->lines[i].line);
    //write(STDOUT_FILENO, fileInfo->lines[i].line,strlen( fileInfo->lines[i].line));
    //write(STDOUT_FILENO,"\n",1);
    if (i == fileInfo->numberOfLinesWithPattern - 1)
    {
      printf("\n");
      //write(STDOUT_FILENO, fileInfo->filename,strlen( fileInfo->filename));
      //write(STDOUT_FILENO,"\n",1);
    }
	}
}
