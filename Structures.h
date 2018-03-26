#include "libraries.h"

typedef struct
{
	int ignoreCase;
	int showFileName;
	int showLinesNumber;
	int sowNumberOfLines;
	int patternIsFullWord;
}Flags;


Flags* create_Flags(int ignoreCase,int showFileName,int showLinesNumber,int sowNumberOfLines,int patternIsFullWord);

int alloc_Flags(Flags** flags);

void free_Flags(Flags** flags);

//nao esta a ver a ordem
Flags* createAndSet_Flags(int argc, char* argv[]);


typedef struct
{
	char* line;
	int lineNumber;
} Line;


Line* create_Line(char* line, int lineNumber, int size);

int alloc_Line(Line** line, int size);

void free_Line(Line** line);


typedef struct
{
	char* filename;
	Flags* flags;
	Line** lines;
	int numberOfLinesWithPattern;

} FileInfo;


FileInfo* create_FileInfo(char* filename, Flags* flags);

int alloc_FileInfo(FileInfo** fileInfo,char* filename);

void free_FileInfo(FileInfo** fileInfo);
