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


typedef struct
{
	char* filename;
	Flags* flags; // pointer para poder ser alterado
	Line* lines;
	int numberOfLinesWithPattern;

} FileInfo;
