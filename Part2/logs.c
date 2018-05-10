
#include "logs.h"

static int fd;

int open_slog()
{
	fd = open("slog.txt",O_WRONLY | O_CREAT, 0644);
	if (fd == ERROR)
	{
		return ERROR;
	}
	return OK;
}

void close_slog()
{
	close(fd);
}


void  write_to_slog(pid_t pid, int to_number, Request* request, Answer* answer)
{
	char to_write[1500];
	sprintf(to_write, "%.*d-%.*d-%.*d: ", 2, to_number,WIDTH_PID,pid,2,request->num_wanted_seats);

	for (int i = 0; i < request->size; i++)
	{
		char temp[9];
		if(i == request->size -1)
			sprintf(temp, "%.*d - ",WIDTH_SEAT,request->pref_seat_list[i]);
		else
			sprintf(temp, "%.*d ",WIDTH_SEAT,request->pref_seat_list[i]);
		strcat(to_write,temp);
	}

	switch(answer->valid_request)
	{
		case MAX:
			strcat(to_write, "MAX\n");
			break;
		case NST:
			strcat(to_write, "NST\n");
			break;
		case IID:
			strcat(to_write, "IID\n");
			break;
		case ERR:
			strcat(to_write, "ERR\n");
			break;
		case NAV:
			strcat(to_write, "NAV\n");
			break;
		case FUL:
			strcat(to_write, "FUL\n");
			break;
		case OK:
			for (int j = 0; j < answer->num_seats; j++)
			{
				char temp[6];
				if (j == answer->num_seats -1)
					sprintf(temp, "%.*d\n",WIDTH_SEAT,answer->seq[j]);
				else
					sprintf(temp, "%.*d ",WIDTH_SEAT,answer->seq[j]);
				strcat(to_write,temp);
			}
			break;
		}
		write_string_to_slog(to_write);
}


void write_string_to_slog(char* str)
{
	write(fd, str, strlen(str));
}

void write_open_ticketOffice(int num)
{
	char to_write[12];
	sprintf(to_write, "%.*d-OPEN\n",2, num);
	write_string_to_slog(to_write);
}


void write_close_ticketOffice(int num)
{
	char to_write[12];
	sprintf(to_write, "%.*d-CLOSED\n", 2,num);
	write_string_to_slog(to_write);
}



int write_to_clog(pid_t pid, Answer* answer)
{
	int fd2;
	fd2 = open("clog.txt",O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd2 == ERROR)
	{
		return ERROR;
	}
	char to_write[1000];
	int s = FALSE;

	switch(answer->valid_request)
	{
		case MAX:
			sprintf(to_write,"%.*d MAX\n",WIDTH_PID, pid);
			break;
		case NST:
			sprintf(to_write,"%.*d NST\n", WIDTH_PID,pid);
			break;
		case IID:
			sprintf(to_write,"%.*d IID\n",WIDTH_PID, pid);
			break;
		case ERR:
			sprintf(to_write,"%.*d ERR\n",WIDTH_PID, pid);
			break;
		case NAV:
			sprintf(to_write,"%.*d NAV\n", WIDTH_PID,pid);
			break;
		case FUL:
			sprintf(to_write,"%.*d FUL\n",WIDTH_PID, pid);
			break;
		case OUT:
			sprintf(to_write,"%.*d OUT\n", WIDTH_PID,pid);
			break;
		case OK:
		printf("here\n");
			for (int i = 0; i < answer->num_seats; i++)
			{
				sprintf(to_write, "%.*d %.*d.%.*d %*.d\n",WIDTH_PID,pid, WIDTH_XXNN, i+1, WIDTH_XXNN, answer->num_seats,WIDTH_SEAT,answer->seq[i]);
				write(fd2, to_write, strlen(to_write));
			}
			s = TRUE;
			break;
	}

	if(s == FALSE)
		write(fd2, to_write, strlen(to_write));
	close(fd2);
	return OK;
}


int write_to_sbook(Seat* seats, int num_seats)
{
	int s_fd = open("sbook.txt",O_WRONLY | O_CREAT, 0644);
	if (s_fd == ERROR)
	{
		return ERROR;
	}
	for(int i = 0; i < num_seats; i++)
	{
		if (seats[i].is_free == FALSE)
		{
			char to_write[6];
			sprintf(to_write, "%.*d\n",WIDTH_SEAT,seats[i].number);
			write(s_fd, to_write, strlen(to_write));
		}
	}
	close(s_fd);
	return OK;
}


int write_to_cbook(int* seats, int number_seats)
{
	int c_fd = open("cbook.txt",O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (c_fd == ERROR)
	{
		return ERROR;
	}
	for(int i = 0; i < number_seats; i++)
	{
		char to_write[6];
		sprintf(to_write, "%.*d\n",WIDTH_SEAT,seats[i]);
		write(c_fd, to_write, strlen(to_write));
	}
	close(c_fd);
	return OK;
}
