#include "seat.h"

//Cada  uma destas funções, após a  operação de  leitura/escrita do conteúdo de
//seats, deve  invocar dentro na  secção  crítica  respetiva uma function-like  macro, DELAY(),
//definida  usando  a  diretiva: #define DELAY()

int isSeatFree(Seat *seats, int seatNum)
{
  DELAY(1);
  return(seats[seatNum-1].is_free);
}

void bookSeat(Seat *seats, int seatNum, int clientId)
{
    DELAY(1.5);
    seats[seatNum-1].clientId = clientId;
    seats[seatNum-1].is_free = FALSE;

}

void freeSeat(Seat *seats, int seatNum)
{
    DELAY(1.5);
    seats[seatNum-1].clientId = -1;
    seats[seatNum-1].is_free = TRUE;
  
}

void freeAllSeats(Seat *seats, int* seq, int size)
{
  for (int i = 0; i < size; i++)
  {
    freeSeat(seats, seq[i]);
  }
}
