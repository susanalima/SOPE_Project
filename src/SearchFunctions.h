
#include "Structures.h"


int LineSearch(Flags* flags, char* pattern, char* line);

int WordSearch(char* pattern, char* line);

int FileSearch(FileInfo* fileInfo, char* pattern);

int DirectorySearch(Flags* flags, char*pattern, char* path);

									/*******AUXILIAR FUNCTIONS**********/

void PrintFileInfo(FileInfo* fileInfo);
