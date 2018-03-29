#include "libraries.h"

typedef struct
{
	char* string;
	int size;
}String;

typedef struct
{
	int ignoreCase;
	int showFileName;
	int showLinesNumber;
	int sowNumberOfLines;
	int patternIsFullWord;
}Flags;

//nao esta a ver a ordem
int set_Flags(int argc, char* argv[],Flags *flags);


typedef struct
{
	char* line;
	int lineNumber;
} Line;


Line * create_Line(char* line, int lineNumber, int size);

int alloc_Line(Line** line, int size);

void free_Line(Line** line);


typedef struct
{
	char* filename;
	Flags* flags; // pointer para poder ser alterado
	Line** lines;
	int numberOfLinesWithPattern;

} FileInfo;


FileInfo* create_FileInfo(char* filename, Flags* flags);

int alloc_FileInfo(FileInfo** fileInfo,char* filename);

void free_FileInfo(FileInfo** fileInfo);
