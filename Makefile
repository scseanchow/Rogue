INC = -Iinclude

all:rogue

rogue: src/main.o src/a3functions.o
	gcc src/main.o src/a3functions.o -std=c99 -Wall -lncurses -o rogue

src/main.o: src/main.c include/functions.h
	gcc -c src/main.c $(INC) -o src/main.o 

src/a3functions.o: src/a3functions.c include/functions.h
	gcc -c src/a3functions.c $(INC) -o src/a3functions.o

clean:
	rm src/main.o src/a3functions.o

run:
	./rogue assets/hard.txt