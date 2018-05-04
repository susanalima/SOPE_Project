#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <math.h>

 /*
 * @brief Successful function
 */
#ifndef macros
#define OK 0
#define READ 0
#define WRITE 1

 /*
 * @brief Unsuccessful function
 */
#define ERROR -1

 /*
 * @brief True function
 */
#define TRUE 1


 /*
 * @brief False function
 */
#define FALSE 0
#endif
void writeLogFile(pid_t pid,char* string);
