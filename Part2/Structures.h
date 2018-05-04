#include "libraries.h"


typedef struct
{
	pid_t pid;
	int time_out;
  int num_wanted_seats;
  char* pref_seat_list; //passar como array de inteiros
	//int* pref_seat_list;
} Request;


typedef struct
{
	int valid_request;
	int num_seats;
	//int* seq;
} Answer;


typedef struct
{
	int number;
	int is_free; // TRUE OR FALSE
	int clientId;
} Seat;
