
#include "logs.h"

//todo
int write_to_slog(char* str)
{
	int fd;
	fd = open("slog.txt",O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		return ERROR;
	}
	write(fd, str, strlen(str));
	close(fd);
  	return OK;
}

int write_open_ticketOffice(int num)
{
	char to_write[1000];
	sprintf(to_write, "%2.d-OPEN", num);
	return write_to_slog(to_write);
}


int write_close_ticketOffice(int num)
{
	char to_write[1000];
	sprintf(to_write, "%2.d-CLOSED", num);
	return write_to_slog(to_write);
}





int write_to_clog(pid_t pid, Answer* answer)
{
	int fd;
	char to_write[1000];
	int s = FALSE;

	fd = open("clog.txt",O_WRONLY | O_CREAT, 0644);
	if (fd == -1)
	{
		return ERROR;
	}
	//FALTA A CENA DA PRECISAO
	switch(answer->valid_request)
	{
		case MAX:
			sprintf(to_write,"%.*d MAX",WIDTH_PID, pid);
			break;
		case NST:
			sprintf(to_write,"%.*d NST", WIDTH_PID,pid);
			break;
		case IID:
			sprintf(to_write,"%.*d IID",WIDTH_PID, pid);
			break;
		case ERR:
			sprintf(to_write,"%.*d ERR",WIDTH_PID, pid);
			break;
		case NAV:
			sprintf(to_write,"%.*d NAV", WIDTH_PID,pid);
			break;
		case FUL:
			sprintf(to_write,"%.*d FUL",WIDTH_PID, pid);
			break;
		case OUT:
			sprintf(to_write,"%.*d OUT", WIDTH_PID,pid);
			break;
		case OK:
			for (int i = 0; i < answer->num_seats; i++)
			{
				sprintf(to_write, "%.*d %.*d.%.*d %*.d",WIDTH_PID,pid, WIDTH_XXNN, i+1, WIDTH_XXNN, answer->num_seats,WIDTH_SEAT,answer->seq[i]);
				write(fd, to_write, strlen(to_write));
				
			}
			s = TRUE;
			break;
	}

	if(s ==FALSE)
		write(fd, to_write, strlen(to_write));
	//write(fd, "\n",2);
	close(fd);
	return OK;
}
