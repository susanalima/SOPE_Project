#include "libraries.h"
#include "SearchFunctions.h"

//isto depois tem de sair da main...
int main (int argc, char *argv[]) {

	Flags* flags;
	char* pattern ;
	char* fileName ;

	if (argc < 3)
	{
		perror("ERROR");
		exit(1);
	}

		pattern = argv[argc-2];
		fileName = argv[argc-1];
		flags = createAndSet_Flags(argc,argv);
		if (flags == NULL) //-r
		{
			printf("work in progress...\n");
			return 0;
		}

	FileInfo* fileInfo;
	fileInfo = create_FileInfo(fileName, flags);

	int s = FileSearch(fileInfo,pattern);
	if (s == ERROR)
		exit(2);
	PrintFileInfo(fileInfo);
	free_FileInfo(&fileInfo);
	return 0;
}
