#ifndef __SEANCHOW_FUNCTIONS__
#define __SEANCHOW_FUNCTIONS__

/*
The struct roomInfo is used to store a 2-d array containing the location of various X,Y locations of the doors and the currentRoom the hero is in
*/
typedef struct RoomInfo
{
  int doorX[7][4];
  int doorY[7][4];
  int currentRoom;
  int shortestRoom;

}Room;
/*
The struct hero info is used to store values related to the hero's stats
*/
typedef struct HeroInfo
{
	int potionCount;
	int heroHP;
	int heroAttack;
	int itemCount;
	int itemType[5];
	int combatWin;
	int goldCount;
	int inHallway;
}Hero;

/*
The struct monster info is used to store X,Y locations of all the monsters as well as the number of monsters of each type
*/
typedef struct monsterInfo
{
	int batCount;
	int batX[10];
	int batY[10];
	int batHP[10];
	int snakeCount;
	int snakeX[10];
	int snakeY[10];
	int snakeHP[10];
	int zombieCount;
	int zombieX[10];
	int zombieY[10];
	int zombieHP[10];
	int aquatorCount;
	int aquatorX[10];
	int aquatorY[10];
	int aquatorHP[10];
	int trollCount;
	int trollX[10];
	int trollY[10];
	int trollHP[10];
	int cycleNumber;
}Monster;

/**
 * readData
 * Reads in the data file and calls DrawRoom, DrawSymbol, spawnHero, returnXY
 * IN: char* fileName - name of text file,  Room *room2 - struct containing door locations
 * OUT: N/A
 * POST: N/A
 * ERROR: If data file fails to load, user will be prompted
 */

void readData(char* fileName, Room *room2, Monster *monsterInfo);
/**
 * ReturnXY
 * 
 * IN: char* str - represents string from fgets, int i - index integer, yptr, xptr = pointers to x,y coordinates
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */

void returnXY(char* str, int i, int* yPTR, int* xPTR);
/**
 * drawRoom
 * Draws the floor and walls of the room 
 * IN: int height, int width - represents height/width, int offsetWidth, int offsetHeight - represents # required to print in proper room
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */

void drawRoom(int height, int width, int offsetWidth, int offsetHeight);
/**
 * drawSymbol
 * Draws the symbol at given X,Y coordinate in appropiate room
 * IN: int x, int y - represents x/y location, int offsetWidth, int offsetHeight - represents # required to print in proper room
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void drawSymbol(int x, int y,int offsetWidth, int offsetHeight, char symbol);

/**
 * drawDoor
 * Draws the door in the room, as well as place the location in a struct
 * IN: int x - represents the location of the door on the wall, int offsetWidth/offsetHeight - represents # required to print in proper room
 * char direction - repesents cardinal direction, Room *room2 - struct int doorCount, roomNo - indexes for placing information into room2 struct
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void drawDoor(int x, int offsetWidth, int height, int width, int offsetHeight, char direction, Room *room2, int doorCount, int roomNo);

/**
 * initializeGame
 * Initializes the game 
 * IN: N/A
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void initializeGame(Hero *heroInventory, Monster *monsterInfo);

/**
 * gameLoop
 * Dictates flow of the game, calls movecursors, exits game upon Q key being pressed
 * IN: int* goldCount - pointer to players goldSum,  Room *room2 - struct that holds doorLocations
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void gameLoop(Room *room2, Hero *heroInventory, Monster *monsterInfo);

/**
 * endGame
 * Calls end of the game and runs closing functions, prints out gold amount
 * IN: int* goldCount - pointer to players goldSum
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void endGame(Room *room2, Hero *heroInventory, Monster *monsterInfo);



/**
 * moveCursor
 * Controls movement of character, checks for collision or interactions with objects
 * IN: char input - takes in WASD movement, int* goldCount - pointer to players goldSum, Room *room2 - struct that holds doorLocations
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void moveCursor(char input, Room *room2, Hero *heroInventory, Monster *monsterInfo);

/**
 * getGold
 * Generates random # and gives it to gold pointer
 * IN: char goldSize - represents large or small gold, int* goldCount - pointer to players goldSum
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void getGold (char goldSize, Hero *heroInventory);

/**
 * spawnHero
 * places hero and moves the cursor to where the hero should start playing the game
 * IN: int x, int y - represents location for hero spawn
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void spawnHero(int x,int y);

/**
 * Notification Bar
 * Updates the notification bar if something is triggered
 * IN: char state, char weaponType
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void notificationBar(char state, char weaponType);

/**
 * Use Potion
 * Uses a hero potion, restoring hero's hp
 * IN: Hero *heroInventory
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void usePotion (Hero *heroInventory);

/**
 * Item Pickup
 * Picks up an item and triggers applicable notification
 * IN: Hero *heroInventory, char state (character of what the hero stepped on)
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void itemPickup(Hero *heroInventory, char state);

/**
 * Status Bar
 * Picks up an item and triggers applicable notification
 * IN: Hero *heroInventory, char state (character of what the hero stepped on)
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void statusBar(Hero *heroInventory);

/**
 * Monster Patrol
 * Moves monsters along their respective patrol routes upon any user input
 * IN: Monster *monsterInfo
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void monsterPatrol(Monster *monsterInfo);

/**
 * Combat Phase
 * Begins fight between monster/player and updates status bar / hero stats / monster hp
 * IN: Monster *monsterInfo, char state, int x, int y (represent coordinates of hero), room *room2
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void combatPhase (Monster *monsterInfo, char state, int x, int y, Hero *heroInventory, Room *room2);

/*
 * checkDeath
 * Simple function which checks if the hero is dead (hp < 0)
 * IN:Room *room2, Hero *heroInventory, Monster *monsterInfo
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void checkDeath (Room *room2, Hero *heroInventory, Monster *monsterInfo);

/*
 * drawHallway
 * Draws the hallways connecting the doors
 * IN:Room *room2, int offsetWidth, int offsetHeight
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void drawHallway(Room *room2, int offsetWidth, int offsetHeight);

/*
 * printItems
 * Prints the items the hero collected
 * IN:Room *room2, Hero *heroInventory, Monster *monsterInfo
 * OUT: N/A
 * POST: N/A
 * ERROR: N/A
 */
void printItems(Room *room2, Hero *heroInventory, Monster *monsterInfo);
#endif