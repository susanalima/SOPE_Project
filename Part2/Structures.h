#ifndef STRUCTURES
#define STRUCTURES


#include "libraries.h"


typedef struct
{
	pid_t pid;
	int time_out;
  int num_wanted_seats;
	int pref_seat_list[MAX_CLI_SEATS];
	int size;
} Request;


typedef struct
{
	int valid_request;
	int num_seats;
	int seq[MAX_CLI_SEATS];
} Answer;


typedef struct
{
	int number;
	int is_free;
	int clientId;
} Seat;


#endif
