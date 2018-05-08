
#include "logs.h"

//todo
int write_to_slog(pid_t pid, Answer* answer)
{
  return OK;
}
int write_to_clog(pid_t pid, Answer* answer)
{
	int fd;
	char to_write[1000];
	//FALTA A CENA DA PRECISAO
	switch(answer->valid_request)
	{
		case MAX:
			sprintf(to_write,"%d MAX", pid);
			break;
		case NST:
			sprintf(to_write,"%d NST", pid);
			break;
		case IID:
			sprintf(to_write,"%d IID", pid);
			break;
		case ERR:
			sprintf(to_write,"%d ERR", pid);
			break;
		case NAV:
			sprintf(to_write,"%d NAV", pid);
			break;
		case FUL:
			sprintf(to_write,"%d FUL", pid);
			break;
		case OK:
			sprintf(to_write,"%d OK", pid); // NAO E ISTO QUE E PARA ESCREVER
			break;
	}

	fd = open("clog.txt",O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		return ERROR;
	}
	write(fd, to_write, strlen(to_write));
	//write(fd, "\n",2);
	close(fd);
	return OK;
}
