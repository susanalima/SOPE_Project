SRC=src/*.c

all:
	gcc -Wall $(SRC) -o run

clean:
	rm run
