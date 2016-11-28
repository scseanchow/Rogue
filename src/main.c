#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <time.h>

int main(int argc, char *argv[])
{	
	srand(time(NULL));

	Room *room2 = (Room*)malloc(sizeof(Room));
	Hero *heroInventory = (Hero*)malloc(sizeof(Hero));
	Monster *monsterInfo = (Monster*)malloc(sizeof(Monster));

    initializeGame(heroInventory,monsterInfo);
    noecho(); // no echo char to screen
	cbreak(); // in/out asap

	readData(argv[1],room2,monsterInfo);
    gameLoop(room2,heroInventory,monsterInfo);
	endGame(room2,heroInventory,monsterInfo);
	free(room2);
	free(heroInventory);
	free(monsterInfo);
	return 0;
}

