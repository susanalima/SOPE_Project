SRC=*.c
SRCSERVER=server.c seat.c logs.c
SRCCLIENT=client.c seat.c logs.c

all:
	gcc -Wall $(SRCSERVER) -o server -D_REENTRANT -lpthread -lrt -Wall
	gcc -Wall $(SRCCLIENT) -o client -D_REENTRANT -lpthread -lrt -Wall

clean:
	rm run
