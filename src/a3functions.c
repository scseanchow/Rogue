#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ncurses.h>
#include <ctype.h>
#include "functions.h"
#include <time.h>

void readData(char* fileName, Room *room2, Monster *monsterInfo)
{
	FILE *dataFile;
	dataFile = fopen(fileName,"r");

	// error check for non-existant data file
	if (dataFile == NULL)
	{
		
		endwin();
		printf("Error loading datafile.\n");
		printf("Program will now exit...\n");
		exit(1);
	}

	// variable declarations
	char str[255];
	int roomNo = 1;
	int lineIndex;
	char direction;
	char temp[10];
	int offsetWidth = 10;
	int offsetHeight = 0;
	int width;
	int height;
	int herospawnX;
	int herospawnY;

	// loop to read in line by line of dataFile
	while (fgets(str,255,dataFile) != NULL)
	{	
		char *pos;
		//doorCount represents # of doors per room , resets to 0 when reading in new room
		int doorCount;
		doorCount = 0;

		// sets temp back to nothing
		memset(temp,'\0',10);
		
		// replaces null terminator with a space to help returnXY funtion work properly
		if ((pos=strchr(str, '\n')) != NULL || (pos=strchr(str, '\0')) != NULL )
		{
			*pos = ' ';
		}
		// for loop to loop through every character of the string
		for (int i = 0; i < strlen(str); i++)
		{	

			int x = 0;
			int y = 0;
			int tempChar = 0;

			//switch statement for str[i], looks for identifiers
			switch (str[i])
			{
				//dimensions for room
				case 'X':

					// this logic test to see if the dimensions of the room are two digits or one digit
					// if the number two spaces to the left is a digit, the first number is a two digit number
					if (isdigit(str[i-2]))
					{
						height = ((str[i-2] - '0') * 10) + (str[i-1] - '0');
						printf("width: %d\n",width);
					}
					else
					{
						height = (str[i-1] - '0');
						printf("width: %d\n",width);
					}
					// if the number two spaces to the right of the X is a digit, the second number is a two digit number
					if (isdigit(str[i+2]))
					{
						width = ((str[i+1] - '0') * 10) + (str[i+2] - '0');
						printf("height: %d\n",height);
					}
					else
					{
						width = (str[i+1] - '0');
						printf("height: %d\n",height);
					}

				// resets offsetWidth back to 0 when printing out the 4,5,6th room 
				if (roomNo == 4)
				{
					offsetWidth = 10;
				}

				//looks for the room with the largest height and sets offsetHeight to that
				if (height > offsetHeight)
				{	
					offsetHeight = height+15;	
				}
				else
				{
					room2->shortestRoom = height;
				}


				// if room #3 > 3, draws the room with an offsetHeight (displaces it down + 4 so it gives it some room)
				if (roomNo > 3)
				{
				
				drawRoom(height, width, offsetWidth,offsetHeight+10);	
				}
				// if not, it just prints it at the y index of 0
				else 
				{	
					if (roomNo == 3)
					{
						drawHallway(room2,offsetWidth+width+3,offsetHeight);
					}
					drawHallway(room2,offsetWidth,offsetHeight);
					drawRoom(height, width, offsetWidth, 10);
				}

				
				break;

				// hero
				case 'h':

					returnXY(str,i,&y,&x);
					// if rooom number is greater than 3, it must account for the offsetheight + 4 when finding the heroSpawn
					if (roomNo > 3)
					{
						herospawnY = y+offsetHeight+10;
						herospawnX = x+offsetWidth;
					}
					
					else
					{
						herospawnY = y+10;
						herospawnX = x+offsetWidth;
					}
					// sets the currentRoom to the room the hero spawns in for the Room struct
					room2->currentRoom = roomNo;

				break;

				// stairs going down
				case 'z':

					returnXY(str,i,&y,&x);
					if (roomNo > 3)
					{
						drawSymbol(x,y,offsetWidth,offsetHeight+10,'<');
					}
					else
					{
						drawSymbol(x,y,offsetWidth,10,'<');
					}

				break;

				// stairs going up
				case 'a':

					returnXY(str,i,&y,&x);
					if (roomNo > 3)
					{
						drawSymbol(x,y,offsetWidth,offsetHeight+10,'>');
					}
					else
					{
						drawSymbol(x,y,offsetWidth,10,'>');
					}

				break;

				// small gold
				case 'g':

				returnXY(str,i,&y,&x);
					if (roomNo > 3)
					{
						drawSymbol(x,y,offsetWidth,offsetHeight+10,'*');
					}
					else
					{
						drawSymbol(x,y,offsetWidth,10,'*');
					}
					
				break;

				// big gold	
				case 'G':

					returnXY(str,i,&y,&x);
					if (roomNo > 3)
					{
						drawSymbol(x,y,offsetWidth,offsetHeight+10,'8');
					}
					else
					{
						drawSymbol(x,y,offsetWidth,10,'8');
					}

				break;

				// common weapon
				case 'w':	

					if (str[i-1] == 'd')
					{
						break;
					}

					returnXY(str,i,&y,&x);
					if (roomNo > 3)
					{
						drawSymbol(x,y,offsetWidth,offsetHeight+10,')');
					}
					else
					{
						drawSymbol(x,y,offsetWidth,10,')');
					}

				break;

				// rare Weapon
				case 'W':
					returnXY(str,i,&y,&x);
					if (roomNo > 3)
					{
						drawSymbol(x,y,offsetWidth,offsetHeight+10,'(');
					}
					else
					{
						drawSymbol(x,y,offsetWidth,10,'(');
					}
				break;

				// weak monster
				case 'm':
					returnXY(str,i,&y,&x);
					//randomly generates a # from 1-4 
					int randomMonster = (rand() % 4) + 1;
					if (roomNo > 3)
					{
						// determines which monster should be spawned
						if (randomMonster == 1)
						{
							drawSymbol(x,y,offsetWidth,offsetHeight+10,'Z');
							monsterInfo->zombieCount++;
							monsterInfo->zombieX[monsterInfo->zombieCount] = x+offsetWidth+1;
							monsterInfo->zombieY[monsterInfo->zombieCount] = y+offsetHeight+11;
						}
						else if (randomMonster == 2)
						{
							drawSymbol(x,y,offsetWidth,offsetHeight+10,'S');
							monsterInfo->snakeCount++;
							monsterInfo->snakeX[monsterInfo->snakeCount] = x+offsetWidth+1;
							monsterInfo->snakeY[monsterInfo->snakeCount] = y+offsetHeight+11;
						}
						else if (randomMonster == 3)
						{
							drawSymbol(x,y,offsetWidth,offsetHeight+10,'B');
							monsterInfo->batCount++;
							monsterInfo->batX[monsterInfo->batCount] = x+offsetWidth+1;
							monsterInfo->batY[monsterInfo->batCount] = y+offsetHeight+11;
						}
						else
						{
							drawSymbol(x,y,offsetWidth,offsetHeight+10,'A');
							monsterInfo->aquatorCount++;
							monsterInfo->aquatorX[monsterInfo->aquatorCount] = x+offsetWidth+1;
							monsterInfo->aquatorY[monsterInfo->aquatorCount] = y+offsetHeight+11;
						}
					}
					else
					{
						if (randomMonster == 1)
						{
							drawSymbol(x,y,offsetWidth,10,'Z');
							monsterInfo->zombieCount++;
							monsterInfo->zombieX[monsterInfo->zombieCount] = x+offsetWidth+1;
							monsterInfo->zombieY[monsterInfo->zombieCount] = y+11;
						}
						else if (randomMonster == 2)
						{
							drawSymbol(x,y,offsetWidth,10,'S');
							monsterInfo->snakeCount++;
							monsterInfo->snakeX[monsterInfo->snakeCount] = x+offsetWidth+1;
							monsterInfo->snakeY[monsterInfo->snakeCount] = y+11;
						}
						else if (randomMonster == 3)
						{
							drawSymbol(x,y,offsetWidth,10,'B');
							monsterInfo->batCount++;
							monsterInfo->batX[monsterInfo->batCount] = x+offsetWidth+1;
							monsterInfo->batY[monsterInfo->batCount] = y+11;
						}
						else
						{
							drawSymbol(x,y,offsetWidth,10,'A');
							monsterInfo->aquatorCount++;
							monsterInfo->aquatorX[monsterInfo->aquatorCount] = x+offsetWidth+1;
							monsterInfo->aquatorY[monsterInfo->aquatorCount] = y+11;
						}
					}
				break;

				// equipment
				case 'e':	

					if (str[i-1] == 'd')
					{
						break;
					}

					returnXY(str,i,&y,&x);
					if (roomNo > 3)
					{
						drawSymbol(x,y,offsetWidth,offsetHeight+10,']');
					}
					else
					{
						drawSymbol(x,y,offsetWidth,10,']');
					}

				break;

				//strong Monster			
				case 'M':
					returnXY(str,i,&y,&x);
					if (roomNo > 3)
					{
						drawSymbol(x,y,offsetWidth,offsetHeight+10,'T');
						monsterInfo->trollCount++;
						monsterInfo->trollX[monsterInfo->trollCount] = x+offsetWidth+1;
						monsterInfo->trollY[monsterInfo->trollCount] = y+offsetHeight+11;
					}
					else
					{
						drawSymbol(x,y,offsetWidth,10,'T');
						monsterInfo->trollCount++;
						monsterInfo->trollX[monsterInfo->trollCount] = x+offsetWidth+1;
						monsterInfo->trollY[monsterInfo->trollCount] = y+11;
					}

				break;

				// potion
				case 'p':

					returnXY(str,i,&y,&x);
					if (roomNo > 3)
					{
						drawSymbol(x,y,offsetWidth,offsetHeight+10,'!');
					}
					else
					{
						drawSymbol(x,y,offsetWidth,10,'!');
					}

				break;

				// door
				case 'd':

					//resets temp back to nothing
					memset(temp,'\0',10);
					//sets lineindex to i+2 which skips the characters 'd' and the direction that follows ie 'de'
					lineIndex = i+2;
					tempChar = 0;

					//sets direction to the character that follows d
					direction = str[i+1];

					// scans until it hits a space
					while (str[lineIndex] != ' ')
					{	
						//creates a temporary string with the contents up to the space
						temp[tempChar] = str[lineIndex];
						lineIndex++;
						tempChar++;
					}

					// converts it to an integer
					x = atoi(temp);

				if (roomNo > 3)
				{
					drawDoor(x,offsetWidth,height,width,offsetHeight+10, direction, room2, doorCount,roomNo);
				}
				else
				{
					
					drawDoor(x,offsetWidth,height,width,10, direction,room2, doorCount,roomNo);
				}

				//increments the door count
				doorCount++;

				break;
			}
		}

		// increase offsetwidth + the width of the current room + 10 (for some breathing room)
		offsetWidth += width + 10;
		roomNo++;

	}
	// calls spawnHero which places the hero in the correct room given herospawnX and herospawnY which were triggered in the 'h' case
	spawnHero(herospawnX,herospawnY);
	// closes data file 
	fclose(dataFile);
}
void returnXY(char* str, int i, int* yPTR, int* xPTR)
{

	// declare a temporary character array
	char temp[10];
	int lineIndex;
	int tempChar;

	tempChar = 0;
	// lineIndex is always one step ahead to look for a space
	lineIndex = i+1;

	// loops ahead until a space is found
	while(str[lineIndex] != ' ') 
	{	
		//sets temp to the content of the string taken in from the data file
		temp[tempChar] = str[lineIndex];

		//increments both
		lineIndex++;
		tempChar++;
	}

	//strtoks the string from the comma, converts it to integer and passes value to y and x ptr
	*yPTR = atoi(strtok(temp,","));
	*xPTR = atoi(strtok(NULL,","));
}

void drawRoom(int height, int width, int offsetWidth, int offsetHeight)
{	
	//allows for room for walls
	height = height+2;
	width = width+2;

	for (int i = 0; i < height+2; i++)
	{
		for (int j = 0; j < width+2; j++)
		{ 
			mvaddch(i+offsetHeight-1,j+offsetWidth-1,'#');
		}
	}	

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{ 
			// prints the entire floor first
			mvaddch(i+offsetHeight,j+offsetWidth,'.');

			// if the iteration of i/j coincide with a tile that should be a wall it prints over the floor and prints a wall
			if (j == 0 || j == width-1)
			{
				mvaddch(i+offsetHeight,j+offsetWidth,'|');
			}

			if (i == 0 || i == height-1)
			{
				mvaddch(i+offsetHeight,j+offsetWidth,'-');
			}
		}
	}
	
}
void drawSymbol(int x, int y, int offsetWidth, int offsetHeight, char symbol)
{	
	// prints out the symbol given the proper x,y coordinates 
	mvaddch(y+offsetHeight+1,x+offsetWidth+1,symbol);
}


void drawDoor(int x, int offsetWidth, int height, int width, int offsetHeight, char direction, Room *room2, int doorCount, int roomNo)
{	

	// the location of the door is dependent on the direction of the door (cardinal direction), therefore a switch statement
	// dictates where it should be placed
	switch (direction)
	{
		case 'e':
			mvaddch(x+offsetHeight+1,width+offsetWidth+1,'+');
			room2->doorX[roomNo][doorCount] = offsetWidth+width;
			room2->doorY[roomNo][doorCount] = x+offsetHeight+1;

		break;

		case 'w':
			mvaddch(x+offsetHeight+1,offsetWidth,'+');
			room2->doorX[roomNo][doorCount] = offsetWidth+1;
			room2->doorY[roomNo][doorCount] = x+offsetHeight+1;

		break;

		case 'n':
			mvaddch(offsetHeight,x+offsetWidth+1,'+');
			room2->doorX[roomNo][doorCount] = x+offsetWidth+1;
			room2->doorY[roomNo][doorCount] = offsetHeight+1;

		break;

		case 's':
			mvaddch(offsetHeight+height+1,x+offsetWidth+1,'+');
			room2->doorX[roomNo][doorCount] = x+offsetWidth+1;
			room2->doorY[roomNo][doorCount] = offsetHeight+height;

		break;
	}
}

void initializeGame(Hero *heroInventory, Monster *monsterInfo) {
	
	
	initscr();
	curs_set(0);
	noecho(); 
	cbreak();

	// set base values to monsters/hero
	heroInventory->potionCount = 1;
	heroInventory->heroHP = 50;
	heroInventory->itemCount = 0;
	heroInventory->heroAttack = 5;
	
	for (int i = 1; i < 10; i++)
	{
		monsterInfo->aquatorHP[i] = 5;
		monsterInfo->trollHP[i] = 50;
		monsterInfo->zombieHP[i] = 15;
		monsterInfo->snakeHP[i] = 5;
		monsterInfo->batHP[i] = 2;
	}

	notificationBar('N','N');
	statusBar(heroInventory);
}
void gameLoop(Room *room2, Hero *heroInventory, Monster *monsterInfo)
{	
	char input;
	do {
	input = getch(); // gets user input (character input ie WASD,Q)
	moveCursor(input,room2,heroInventory,monsterInfo); // calls moveCursor, passing the room struct and goldCount
	refresh();
	} while (input != 'q'); // if user types Q game quits
}

void endGame(Room *room2, Hero *heroInventory, Monster *monsterInfo)
{
	// removes ncurses screen
	endwin();
	
	printf("You collected: %d gold.\n",heroInventory->goldCount);
	printItems(room2,heroInventory,monsterInfo);

	free(room2);
	free(heroInventory);
	free(monsterInfo);
	exit(0);

}  
void moveCursor(char input, Room *room2, Hero *heroInventory, Monster *monsterInfo)
{
	int xpos, ypos;
	int oldx, oldy;
	char nextChar;
	getyx(stdscr, ypos, xpos);

	oldx = xpos;
	oldy = ypos;

	// controls movement
	switch(input) {

	case 'w':
		ypos--;
		notificationBar('N','N');

	break;

	case 'a':
		notificationBar('N','N');
		xpos--;
	break;

	case 's':
		notificationBar('N','N');
		ypos++;
	break;

	case 'd':
		notificationBar('N','N');
		xpos++;
	break;

	case 'p': 
	usePotion(heroInventory);
	statusBar(heroInventory);
	default:
	break;
	}
   	nextChar = (char)mvinch(ypos,xpos);

   	// collision 

   	if (nextChar == ' ' || nextChar ==  '-' || nextChar ==  '|')
   	{
   		mvaddch(oldy, oldx, '@');
   		statusBar(heroInventory);
   		monsterPatrol(monsterInfo);
   		move(oldy, oldx);
   	}
   	else if (nextChar == 'A')
   	{
   		 combatPhase(monsterInfo,nextChar,xpos,ypos,heroInventory,room2);
   		 statusBar(heroInventory);
   		 if (heroInventory->combatWin == 1)
   		 {
	 		mvaddch(ypos, xpos, '@');
   			mvaddch(oldy, oldx, '.');
   			monsterPatrol(monsterInfo);
   			move(ypos, xpos);
   		 }
   		 else
   		 {
	   		mvaddch(oldy, oldx, '@');
	   		statusBar(heroInventory);
	   		monsterPatrol(monsterInfo);
	   		move(oldy, oldx);
   		 }
   	}
	else if (nextChar == 'B')
   	{
   		 combatPhase(monsterInfo,nextChar,xpos,ypos,heroInventory,room2);
   		 statusBar(heroInventory);
   		 if (heroInventory->combatWin == 1)
   		 {
	 		mvaddch(ypos, xpos, '@');
   			mvaddch(oldy, oldx, '.');
   			monsterPatrol(monsterInfo);
   			move(ypos, xpos);
   		 }
   		 else
   		 {
	   		mvaddch(oldy, oldx, '@');
	   		statusBar(heroInventory);
	   		monsterPatrol(monsterInfo);
	   		move(oldy, oldx);
   		 }
   		 
   	}
   	else if (nextChar == 'S')
   	{
   		 combatPhase(monsterInfo,nextChar,xpos,ypos,heroInventory,room2);
   		 statusBar(heroInventory);
   		 if (heroInventory->combatWin == 1)
   		 {
	 		mvaddch(ypos, xpos, '@');
   			mvaddch(oldy, oldx, '.');
   			monsterPatrol(monsterInfo);
   			move(ypos, xpos);
   		 }
   		 else
   		 {
	   		mvaddch(oldy, oldx, '@');
	   		statusBar(heroInventory);
	   		monsterPatrol(monsterInfo);
	   		move(oldy, oldx);
   		 }
   	}

	else if (nextChar == 'Z')
   	{
   		 combatPhase (monsterInfo,nextChar,xpos,ypos,heroInventory,room2);
   		 statusBar(heroInventory);
   		 if (heroInventory->combatWin == 1)
   		 {
	 		mvaddch(ypos, xpos, '@');
   			mvaddch(oldy, oldx, '.');
   			monsterPatrol(monsterInfo);
   			move(ypos, xpos);
   		 }
   		 else
   		 {
	   		mvaddch(oldy, oldx, '@');
	   		statusBar(heroInventory);
	   		monsterPatrol(monsterInfo);
	   		move(oldy, oldx);
   		 }
   	}

   	else if (nextChar == 'T')
   	{
   		 combatPhase (monsterInfo,nextChar,xpos,ypos,heroInventory,room2);
   		 statusBar(heroInventory);
   		 if (heroInventory->combatWin == 1)
   		 {
	 		mvaddch(ypos, xpos, '@');
   			mvaddch(oldy, oldx, '.');
   			monsterPatrol(monsterInfo);
   			move(ypos, xpos);
   		 }
   		 else
   		 {
	   		mvaddch(oldy, oldx, '@');
	   		statusBar(heroInventory);
	   		monsterPatrol(monsterInfo);
	   		move(oldy, oldx);
   		 }
   	}

   	else if (nextChar == '>' || nextChar == '<')
   	{	
   		//stairs trigger the game ending
   		endGame(room2,heroInventory,monsterInfo);
   	}
	else if (nextChar == '*' || nextChar == '8')
	{
		// if gold is hit, calls getGold and passes size of the gold
		switch (nextChar)
		{
			case '*':
			notificationBar('*','N');
			break;
			case '8':
			notificationBar('8','N');
			break;
		}
		getGold(nextChar, heroInventory);
		mvaddch(ypos, xpos, '@');
		mvaddch(oldy, oldx, '.');
		statusBar(heroInventory);
		monsterPatrol(monsterInfo);
		move(ypos, xpos);
	}
	else if (nextChar == '+')
	{	
		
		mvaddch(oldy,oldx,'.');
		mvaddch(ypos,xpos,'@');
		statusBar(heroInventory);
		notificationBar('+','N');
		monsterPatrol(monsterInfo);
		


		if (heroInventory->inHallway == 0)
		{
			mvaddch(oldy,oldx,'.');
			heroInventory->inHallway = 1;
		}
		if (heroInventory->inHallway == 2)
		{
			mvaddch(oldy,oldx,'#');
			heroInventory->inHallway = 3;
		}
		move(ypos,xpos);
			
	}
	else if (nextChar == '#')
	{	
		// door is hit, calls roomteleport and replaces the player with a .
		
		mvaddch(oldy, oldx, '#');
		mvaddch(ypos, xpos, '@');
		notificationBar('N','N');
		monsterPatrol(monsterInfo);
		if (heroInventory->inHallway == 1)
		{
			mvaddch(oldy,oldx,'+');
			heroInventory->inHallway = 2;
		}
		
		move(ypos,xpos);
	}
	else if (nextChar == '!')
	{
		notificationBar('!','N');
		heroInventory->potionCount++;
		mvaddch(ypos, xpos, '@');
   		mvaddch(oldy, oldx, '.');
   		statusBar(heroInventory);
   		monsterPatrol(monsterInfo);
   		move(ypos, xpos);
	}
	else if (nextChar == ']')
	{
		heroInventory->itemCount++;
		itemPickup(heroInventory, ']');
		mvaddch(ypos, xpos, '@');
   		mvaddch(oldy, oldx, '.');
   		statusBar(heroInventory);
   		monsterPatrol(monsterInfo);
   		move(ypos, xpos);
   		
	}

	else if (nextChar == '(')
	{
		heroInventory->itemCount++;
		itemPickup(heroInventory, '(');
		mvaddch(ypos, xpos, '@');
   		mvaddch(oldy, oldx, '.');
   		statusBar(heroInventory);
   		monsterPatrol(monsterInfo);
   		move(ypos, xpos);
   		
	}
	else if (nextChar == ')')
	{
		heroInventory->itemCount++;
		itemPickup(heroInventory, ')');
		mvaddch(ypos, xpos, '@');
   		mvaddch(oldy, oldx, '.');
   		statusBar(heroInventory);
   		monsterPatrol(monsterInfo);
   		move(ypos, xpos);
   		
	}
   	else if (ypos == oldy && xpos == oldx)
   	{
		mvaddch(ypos, xpos, '@');
		monsterPatrol(monsterInfo);
		move(ypos, xpos);
   	}	
   	else if (nextChar == '.')
   	{
	mvaddch(ypos, xpos, '@');
	mvaddch(oldy, oldx, '.');
	statusBar(heroInventory);
	monsterPatrol(monsterInfo);

	if (heroInventory->inHallway == 3)
	{
		mvaddch(oldy,oldx,'+');
		heroInventory->inHallway = 0;
	}
	move(ypos, xpos);
   	}
}

void getGold (char goldSize, Hero *heroInventory)
{
	if (goldSize == '*')
	{	
		// RNGS a number between 0-50
		heroInventory->goldCount += rand() % 51;
	}
	else
	{
		//rngs a number from 50-250
		heroInventory->goldCount += (rand() % 201 ) + 50;
	}
}	
void spawnHero(int x,int y)
{

	// moves cursor and adds hero spot
	mvaddch(y+1, x+1, '@');
	move(y+1,x+1);
}

void notificationBar(char state, char state2)
{	
	char temp[75];
	
	switch(state) {
	case '+':
		strcpy(temp, "Hero opened a door!                ");
	break;

	case 'p':
		strcpy(temp, "Hero recovered full health!        ");
	break;

	case '0':
		strcpy(temp, "You have no potions!               ");
	break;

	case '!':
		strcpy(temp, "You picked up a potion!            ");
	break;

	case '8':
		strcpy(temp, "You picked up a big gold!          ");
	break;

	case '*':
		strcpy(temp, "You picked up a small gold!        ");
	break;
	case ']':
		strcpy(temp, "You picked up equipment!           ");
	break;

	case ')':
		strcpy(temp, "You picked up a weak weapon!       ");
	break;

	case '(':
		strcpy(temp, "You picked up a rare weapon!       ");
	break;

	case 'a':
			switch (state2){
				case 1:
				strcpy(temp, "You did 1 damage!         ");
				break;

				case 2:
				strcpy(temp, "You did 2 damage!         ");
				break;

				case 3:
				strcpy(temp, "You did 3 damage!         ");
				break;

				case 4:
				strcpy(temp, "You did 4 damage!         ");
				break;

				case 5:
				strcpy(temp, "You did 5 damage!         ");
				break;
			}
	break;
	case 'x':
		switch (state2){
			case ')':
			strcpy(temp, "You stepped on a common weapon, and broke it!       ");
			break;

			case ']':
			strcpy(temp, "You stepped on equipment, and broke it!             ");
			break;

			case '(':
			strcpy(temp, "You stepped on a rare weapon, and broke it!         ");
			break;
		}
		
	break;

	case 'd':
		switch (state2){
			case 5:
			strcpy(temp, "You took 5 damage!                                 ");
			break;

			case 1:
			strcpy(temp, "You lost 1 attack!                                 ");
			break;

		}
	break;

	default:
		strcpy(temp,"No notifications.                                         ");
		
	break;

	}
	int y;
	int x;

	getmaxyx(stdscr,y,x);
	mvprintw(5,11,"%s",temp);
	mvprintw(4,10,"%s","===================================================");
	mvprintw(5,10,"%s","|");
	mvprintw(5,60,"%s","|");
	mvprintw(6,10,"%s","===================================================");
	memset(temp,'\0',75);
	
}

void statusBar(Hero *heroInventory)
{
	int x;
	int y;
	getmaxyx(stdscr,y,x);
	if (heroInventory->itemCount < 5)
	{
		mvprintw(y-2,31,"%s %d %s %d %s %d %s %d %s","Health:",heroInventory->heroHP,"Potions:",heroInventory->potionCount,"Attack:",heroInventory->heroAttack,"Inv:",heroInventory->itemCount,"/ 5      ");
	}
	else
	{
		mvprintw(y-2,31,"%s %d %s %d %s %d %s","Health:",heroInventory->heroHP,"Potions:",heroInventory->potionCount,"Attack:",heroInventory->heroAttack,"Inv: 5 / 5   ");
	}
	mvprintw(y-3,30,"%s","===================================================");
	mvprintw(y-1,30,"%s","===================================================");
}
void usePotion (Hero *heroInventory)
{
	if (heroInventory->potionCount > 0)
	{
		heroInventory->heroHP = 50;
		heroInventory->potionCount--;
		notificationBar('p','N');
	}
	else
	{
		notificationBar('0', 'N');
	}
}
void itemPickup(Hero *heroInventory, char state)
{	
	if (heroInventory->itemCount < 6)
	{
		switch (state){
		case ')':
			notificationBar(')','N');
			heroInventory->itemType[heroInventory->itemCount] = 1;
		break;

		case '(':
			notificationBar('(','N');
				heroInventory->itemType[heroInventory->itemCount] = 2;
		break;

		case ']':
			notificationBar(']','N');
			heroInventory->itemType[heroInventory->itemCount] = 3;
		break;
		}
	}
	else
	{
		notificationBar('x',state);
	}
}
void monsterPatrol(Monster *monsterInfo)
{

	int oldX;
	int oldY;

	
	monsterInfo->cycleNumber++;

	for (int i = 1; i < monsterInfo->batCount+1; i++)
	{
		int randomMovement = rand() % 4;
		oldX = monsterInfo->batX[i];
		oldY = monsterInfo->batY[i];
		
		switch (randomMovement)
		{
			case 0:

				if (mvinch(monsterInfo->batY[i]+1,monsterInfo->batX[i]) == '.')
				{
					mvaddch(oldY,oldX,'.');
					mvaddch(monsterInfo->batY[i]+1,monsterInfo->batX[i],'B');
					monsterInfo->batY[i]++;
				}

			break;

			case 1:
				if (mvinch(monsterInfo->batY[i]-1,monsterInfo->batX[i]) == '.')
				{
					mvaddch(oldY,oldX,'.');
					mvaddch(monsterInfo->batY[i]-1,monsterInfo->batX[i],'B');
					monsterInfo->batY[i]--;

				}
			break;

			case 2:
				if (mvinch(monsterInfo->batY[i],monsterInfo->batX[i]+1) == '.')
				{
					mvaddch(oldY,oldX,'.');
					mvaddch(monsterInfo->batY[i],monsterInfo->batX[i]+1,'B');
					monsterInfo->batX[i]++;
				}
			break;

			case 3:
				if (mvinch(monsterInfo->batY[i],monsterInfo->batX[i]-1) == '.')
				{
					mvaddch(oldY,oldX,'.');
					mvaddch(monsterInfo->batY[i],monsterInfo->batX[i]-1,'B');
					monsterInfo->batX[i]--;
				}
			break;

		}
		
	}
	for (int i = 1; i < monsterInfo->snakeCount+1;i++)
	{
		oldX = monsterInfo->snakeX[i];
		oldY = monsterInfo->snakeY[i];

		switch (monsterInfo->cycleNumber){

			case 1:
			if (mvinch(monsterInfo->snakeY[i],monsterInfo->snakeX[i]+1) == '.')
			{
				mvaddch(oldY,oldX,'.');
				mvaddch(monsterInfo->snakeY[i],monsterInfo->snakeX[i]+1,'S');
				monsterInfo->snakeX[i]++;
			}
			break;

			case 2:
			if (mvinch(monsterInfo->snakeY[i],monsterInfo->snakeX[i]+1) == '.')
			{
				mvaddch(oldY,oldX,'.');
				mvaddch(monsterInfo->snakeY[i],monsterInfo->snakeX[i]+1,'S');
				monsterInfo->snakeX[i]++;
			}
			break;

			case 3:
			if (mvinch(monsterInfo->snakeY[i],monsterInfo->snakeX[i]-1) == '.')
			{
				mvaddch(oldY,oldX,'.');
				mvaddch(monsterInfo->snakeY[i],monsterInfo->snakeX[i]-1,'S');
				monsterInfo->snakeX[i]--;
			}
				
			break;

			case 4:
			if (mvinch(monsterInfo->snakeY[i],monsterInfo->snakeX[i]-1) == '.')
			{
				mvaddch(oldY,oldX,'.');
				mvaddch(monsterInfo->snakeY[i],monsterInfo->snakeX[i]-1,'S');
				monsterInfo->snakeX[i]--;
			}
			break;

		}
	}

	for (int i = 1; i < monsterInfo->zombieCount+1;i++)
	{
		oldX = monsterInfo->zombieX[i];
		oldY = monsterInfo->zombieY[i];

		switch (monsterInfo->cycleNumber){

			case 1:
			case 3:
			if (mvinch(monsterInfo->zombieY[i]-1,monsterInfo->zombieX[i]) == '.')
			{
				mvaddch(oldY,oldX,'.');
				mvaddch(monsterInfo->zombieY[i]-1,monsterInfo->zombieX[i],'Z');
				monsterInfo->zombieY[i]--;
				mvaddch(oldY,oldX,'.');
			}
			break;

			case 2:
			case 4:
			if (mvinch(monsterInfo->zombieY[i]+1,monsterInfo->zombieX[i]) == '.')
			{
				mvaddch(oldY,oldX,'.');
				mvaddch(monsterInfo->zombieY[i]+1,monsterInfo->zombieX[i],'Z');
				monsterInfo->zombieY[i]++;
				mvaddch(oldY,oldX,'.');
			}
			break;

		}
	}

	for (int i = 1; i < monsterInfo->trollCount+1;i++)
	{
		oldX = monsterInfo->trollX[i];
		oldY = monsterInfo->trollY[i];

		switch (monsterInfo->cycleNumber){

			case 1:
			if (mvinch(monsterInfo->trollY[i]-1,monsterInfo->trollX[i]) == '.')
			{
				mvaddch(oldY,oldX,'.');
				mvaddch(monsterInfo->trollY[i]-1,monsterInfo->trollX[i],'T');
				monsterInfo->trollY[i]--;
				
			}
			break;

			case 2:
			if (mvinch(monsterInfo->trollY[i],monsterInfo->trollX[i]+1) == '.')
			{
				mvaddch(oldY,oldX,'.');
				mvaddch(monsterInfo->trollY[i],monsterInfo->trollX[i]+1,'T');
				monsterInfo->trollX[i]++;
				
			}
			break;

			case 3:
			if (mvinch(monsterInfo->trollY[i]+1,monsterInfo->trollX[i]) == '.')
			{
				mvaddch(oldY,oldX,'.');
				mvaddch(monsterInfo->trollY[i]+1,monsterInfo->trollX[i],'T');
				monsterInfo->trollY[i]++;
			}
				
			break;

			case 4:
			if (mvinch(monsterInfo->trollY[i],monsterInfo->trollX[i]-1) == '.')
			{
				mvaddch(oldY,oldX,'.');
				mvaddch(monsterInfo->trollY[i],monsterInfo->trollX[i]-1,'T');
				monsterInfo->trollX[i]--;
			}

			break;

			

		}
		
	}

	if (monsterInfo->cycleNumber == 4)
	{
		monsterInfo->cycleNumber = 0;
	}
}
void combatPhase (Monster *monsterInfo, char state, int x, int y, Hero *heroInventory, Room *room2)
{
	int randomSpeed = rand() % 11;
	int monsterAttack;
	int monsterSpeed;
	int temp;

	heroInventory->combatWin = 0;

	switch (state)
	{
		case 'A':
			monsterAttack = 1;
			monsterSpeed = 	2;
			
			
			for (int i = 1; i < monsterInfo->aquatorCount+1; i++)
			{
				if (monsterInfo->aquatorY[i] == y && monsterInfo->aquatorX[i] == x)
				{

					temp = i;
				}
			}

			if (randomSpeed > monsterSpeed)
			{
				notificationBar('a',heroInventory->heroAttack);
				monsterInfo->aquatorHP[temp] = monsterInfo->aquatorHP[temp] - heroInventory->heroAttack;
				if (monsterInfo->aquatorHP[temp] <= 0)
				{
					heroInventory->combatWin = 1;
				}
			}
			else
			{
				notificationBar('d',monsterAttack);
				if (heroInventory->heroAttack > 1)
				{
				heroInventory->heroAttack--;
				}
			}

		break;

		case 'B':
			monsterAttack = 5;
			monsterSpeed = 4;

			for (int i = 1; i < monsterInfo->batCount+1; i++)
			{
				if (monsterInfo->batY[i] == y && monsterInfo->batX[i] == x)
				{
					temp = i;
				}
			}

			if (randomSpeed > monsterSpeed)
			{
				notificationBar('a',heroInventory->heroAttack);
				monsterInfo->batHP[temp] = monsterInfo->batHP[temp] - heroInventory->heroAttack;
				if (monsterInfo->batHP[temp] <= 0)
				{
					heroInventory->combatWin = 1;
					monsterInfo->batY[temp] = 1000;
					monsterInfo->batX[temp] = 1000;
				}
			}
			else
			{
				notificationBar('d',monsterAttack);
				heroInventory->heroHP = heroInventory->heroHP - monsterAttack;
				checkDeath(room2,heroInventory,monsterInfo);
			}
		break;

		case 'S':
			monsterAttack = 5;
			monsterSpeed = 4;

			for (int i = 1; i < monsterInfo->snakeCount+1; i++)
			{
				if (monsterInfo->snakeY[i] == y && monsterInfo->snakeX[i] == x)
				{
					temp = i;
				}
			}

			if (randomSpeed > monsterSpeed)
			{
				notificationBar('a',heroInventory->heroAttack);
				monsterInfo->snakeHP[temp] = monsterInfo->snakeHP[temp] - heroInventory->heroAttack;
				if (monsterInfo->snakeHP[temp] <= 0)
				{
					heroInventory->combatWin = 1;
					monsterInfo->snakeY[temp] = 1000;
					monsterInfo->snakeX[temp] = 1000;

				}
			}
			else
			{
				notificationBar('d',monsterAttack);
				heroInventory->heroHP = heroInventory->heroHP - monsterAttack;
				checkDeath(room2,heroInventory,monsterInfo);
			}
		break;

		case 'Z':
			monsterAttack = 5;
			monsterSpeed = 2;

			for (int i = 1; i < monsterInfo->zombieCount+1; i++)
			{
				if (monsterInfo->zombieY[i] == y && monsterInfo->zombieX[i] == x)
				{
					temp = i;
				}
			}

			if (randomSpeed > monsterSpeed)
			{
				notificationBar('a',heroInventory->heroAttack);
				monsterInfo->zombieHP[temp] = monsterInfo->zombieHP[temp] - heroInventory->heroAttack;
				if (monsterInfo->zombieHP[temp] <= 0)
				{
					heroInventory->combatWin = 1;
					monsterInfo->zombieY[temp] = 1000;
					monsterInfo->zombieX[temp] = 1000;
				}
			}
			else
			{
				notificationBar('d',monsterAttack);
				heroInventory->heroHP = heroInventory->heroHP - monsterAttack;
				checkDeath(room2,heroInventory,monsterInfo);
			}

		break;

		case 'T':
			monsterAttack = 5;
			monsterSpeed = 3;

			for (int i = 1; i < monsterInfo->trollCount+1; i++)
			{
				if (monsterInfo->trollY[i] == y && monsterInfo->trollX[i] == x)
				{
					temp = i;
				}
			}

			if (randomSpeed > monsterSpeed)
			{
				notificationBar('a',heroInventory->heroAttack);
				monsterInfo->trollHP[temp] = monsterInfo->trollHP[temp] - heroInventory->heroAttack;
				if (monsterInfo->trollHP[temp] <= 0)
				{
					heroInventory->combatWin = 1;
					monsterInfo->trollY[temp] = 1000;
					monsterInfo->trollX[temp] = 1000;
				}
			}
			else
			{
				notificationBar('d',monsterAttack);
				heroInventory->heroHP = heroInventory->heroHP - monsterAttack;
				checkDeath(room2,heroInventory,monsterInfo);
			}

		break;

	}


}
void checkDeath (Room *room2, Hero *heroInventory, Monster *monsterInfo)
{
	if (heroInventory->heroHP <= 0)
	{
		endGame(room2,heroInventory,monsterInfo);
	}	
}

void drawHallway(Room *room2, int offsetWidth, int offsetHeight)
{

	move(offsetHeight+5,0);
	whline(stdscr, '#', offsetWidth+16);
	move(room2->shortestRoom+10,offsetWidth-1);
	wvline(stdscr, '#', offsetHeight+4);  
}
void printItems(Room *room2, Hero *heroInventory, Monster *monsterInfo)
{	
	int randomNum;

	for (int i = 1; i < heroInventory->itemCount+1; i++)
	{
		if (heroInventory->itemType[i] == 1)
		{	
			randomNum = (rand() % 9) + 1;
			printf("You collected a common weapon with attack rating: %d\n",randomNum);
		}
		if (heroInventory->itemType[i] == 2)
		{	
			randomNum = (rand() % 16) + 10;
			printf("You collected a rare weapon with attack rating: %d\n",randomNum);
		}
		if (heroInventory->itemType[i] == 3)
		{	
			randomNum = (rand() % 26) + 1;
			printf("You collected an equipment item with a defence rating: %d\n",randomNum);
		}
	}	
}