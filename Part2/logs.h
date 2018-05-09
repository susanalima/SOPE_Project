

#include "Structures.h"


int open_slog();

void close_slog();

void  write_to_slog(pid_t pid, int to_number, Request* request, Answer* answer);

void write_string_to_slog(char* str);

void write_open_ticketOffice(int num);

void write_close_ticketOffice(int num);

int write_to_clog(pid_t pid, Answer* answer);

int write_to_sbook(Seat* seats, int num_seats);

int write_to_cbook(int* seats, int number_seats);
