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
#include <sys/file.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <semaphore.h>


#ifndef macros
/*
* @brief Successful function
*/
#define OK 0

/*
* @brief Read function
*/
#define READ 0

/*
* @brief Write function
*/
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

/*
-1–a quantidade de lugares pretendidos é superior ao máximo permitido (MAX_CLI_SEATS)
-2–o número de identificadores dos lugares pretendidos não é válido
-3–os identificadores dos lugares pretendidos não são válidos
-4–outros erros nos parâmetros
-5–pelo menos um dos lugares pretendidos não está disponível
-6–sala cheia
*/

#define MAX -1
#define NST -2
#define IID -3
#define ERR -4
#define NAV -5
#define FUL -6


#define MAX_ROOM_SEAT 9999
#define MAX_CLI_SEATS 99
#define WIDTH_PID 5
#define WIDTH_XXNN  5
#define WIDTH_SEAT 4


#endif
