// CSE240 Spring 2023 HW 7 & 8
// Write Alexis Urias
// Write the compiler used: Visual studio

// READ BEFORE YOU START:
// You are given a partially completed program that creates a linked list of maps in a video game.
// The global linked list 'list' is a list of maps with each node being a struct 'mapList'.
// 'mapList' consists of a struct 'map' which has: map name, the number of enemies present, a pointer to the Hero if present, and a linked list of 'enemyList'.
// There is a struct called 'hero', which contains the attributes name, health, attack stat, and location.
// theHero is a global instance of the 'hero' struct. There should only be one hero active at any given time in your code. 
// The linked list of enemies has each node containing the name, health, and attack stat of the enemy.
// HW7 ignores the 'enemy' linked list since there is no operation/manipulation to be done on the 'enemies' list in HW7.
// HW8 has operations/manipulations to be done with the 'enemies' list like add an enemy, remove an enemy, and combat simulation.

// To begin, you should trace through the given code and understand how it works.
// Please read the instructions above each required function and follow the directions carefully.
// If you modify any of the given code, the return types, or the parameters, you risk getting compile error.
// You are not allowed to modify main ().
// You can use string library functions.

// ***** WRITE COMMENTS FOR IMPORANT STEPS OF YOUR CODE. *****
// ***** GIVE MEANINGFUL NAMES TO VARIABLES. *****
// ***** Before implementing any function, see how it is called in executeAction() *****


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 4996) 			// for Visual Studio

#define MAX_NAME 30
int enemyCounter = 0;

// global linked list 'list' contains the list of maps
struct mapList {
	struct map* map;
	struct mapList* next;
} *list = NULL;					// currently empty list



// structure "map" contains the map's name, enemy count, hero pointer, and linked list of enemies
struct map {
	char name[MAX_NAME];
	unsigned int enemyCount;
	struct hero* hero;
	struct enemy* enemyList;		// linked list 'enemies' contains a list of enemies on the map
};

//  structure 'enemy' contains the enemy's stats 
struct enemy {
	char name[MAX_NAME];
	int hp;
	int attack;
	struct enemy* next;
};

// structure 'hero' contains the hero's stats
struct hero {
	char name[MAX_NAME];
	int hp;
	int attack;
	char location[MAX_NAME];
} *theHero = NULL;

// forward declaration of functions (already implmented)
void flushStdIn();
void executeAction(char);
void displayHero();
void displayMapList(struct mapList* tempList);
void displayMapEnemyList(struct mapList* tempList);

// functions that need implementation:
// HW 7
void addMap(char* mapNameInput, unsigned int enemyCount); 						// 15 points
struct map* searchMap(char* mapNameInput);	      							// 10 points
void createHero(char* heroNameInput, unsigned int heroHpInput, unsigned int heroAttackInput); 	// 5 points
void moveHero(char* mapNameInput); 									// 10 points
//HW 8
void addEnemy(char* mapNameInput, char* enemyNameInput, unsigned int enemyHpInput, unsigned int enemyAttackInput);	// 10 points
void removeMap(char* mapNameInput);									// 10 points
int mapCombat(char* mapNameInput);									// mapCombat and combatRecurse are 20 points together
int combatRecurse(struct hero* theHero, struct enemy* enemyList);					// mapCombat and combatRecurse are 20 points together


int main()
{
	char selection = 'a';		// initialized to a dummy value
	do
	{
		printf("\nCSE240 HW 7,8\n");
		printf("HW7:\n");
		printf("\t a: add a new map to the list\n");
		printf("\t d: display map list (no enemies)\n");
		printf("\t b: search for a map on the list\n");
		printf("\t h: create or redo the hero\n");
		printf("\t m: move the hero to a new map\n");
		printf("\t i: display hero information\n");
		printf("\t q: quit\n");
		printf("HW8:\n");
		printf("\t c: add an enemy to a map\n");
		printf("\t l: display maps that have a specific enemy\n");
		printf("\t r: remove a map\n");
		printf("\t f: conduct combat on a map\n");
		printf("\t q: quit\n");
		printf("Please enter your selection:\n");

		selection = getchar();
		flushStdIn();
		executeAction(selection);
	} while (selection != 'q');

	return 0;
}

// flush out leftover '\n' characters
void flushStdIn()
{
	char c;
	do c = getchar();
	while (c != '\n' && c != EOF);
}

// Ask for details from user for the given selection and perform that action
// Read the function case by case
void executeAction(char c)
{
	char mapNameInput[MAX_NAME], enemyNameInput[MAX_NAME], heroNameInput[MAX_NAME];
	unsigned int heroHpInput, heroAttackInput, enemyHpInput, enemyAttackInput;
	int combatOutcome = 0;
	struct mapList* tempList = NULL;

	switch (c)
	{
	case 'a':						// add map
								// input map details from user
		printf("\nPlease enter map's name: ");
		fgets(mapNameInput, sizeof(mapNameInput), stdin);
		mapNameInput[strlen(mapNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchMap(mapNameInput) == NULL)					
		{
			addMap(mapNameInput, 0); //Pass static zero into the function as all maps will start with zero enemies
			printf("\nThe map was successfully added to the list!\n");
		}
		else
			printf("\nThat map is already on the list!\n");
		break;

	case 'd':						// display the map list
		tempList = list;
		displayMapList(list);
		break;

	case 'b':						// search for a map on the list
		printf("\nPlease enter a map's name: ");
		fgets(mapNameInput, sizeof(mapNameInput), stdin);
		mapNameInput[strlen(mapNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchMap(mapNameInput) == NULL)				
			printf("\nThe map does not exist or the list is empty! \n\n");
		else
		{
			printf("\nThe map exists on the list! \n\n");
		}
		break;
	
	case 'h':
		printf("\nPlease enter the hero's name: ");
		fgets(heroNameInput, sizeof(heroNameInput), stdin);
		heroNameInput[strlen(heroNameInput) - 1] = '\0';	// discard the trailing '\n' char
		printf("\nPlease enter the hero's Hp Stat: ");
		scanf("%d", &heroHpInput);	
		flushStdIn();
		printf("\nPlease enter the hero's Attack Stat: ");
		scanf("%d", &heroAttackInput);
		flushStdIn();
		createHero(heroNameInput, heroHpInput, heroAttackInput);
		printf("\nThe Hero was created! \n\n");
		break;

	case 'm':
		printf("\nPlease enter the map's name: ");
		fgets(mapNameInput, sizeof(mapNameInput), stdin);
		mapNameInput[strlen(mapNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchMap(mapNameInput) == NULL)					
			printf("\nThe map does not exist or the list is empty! \n\n");
		else{
			moveHero(mapNameInput);
			printf("\nThe Hero has been moved! \n\n");
		}
		break;

	case 'r':						// remove a map
		printf("\nPlease enter a map's name: ");
		fgets(mapNameInput, sizeof(mapNameInput), stdin);
		mapNameInput[strlen(mapNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchMap(mapNameInput) == NULL)					
			printf("\nThe map does not exist or the list is empty! \n\n");
		else
		{
			removeMap(mapNameInput);
			printf("\nThe map was successfully removed from the list! \n\n");
		}
		break;
	
	case 'i':
		displayHero();
		break;

	case 'c':						// add an enemy
		printf("\nPlease enter the map's name: ");
		fgets(mapNameInput, sizeof(mapNameInput), stdin);
		mapNameInput[strlen(mapNameInput) - 1] = '\0';	// discard the trailing '\n' char

		if (searchMap(mapNameInput) == NULL)					
			printf("\nThe map does not exist or the list is empty! \n\n");
		else
		{
			printf("\nPlease enter the enemy's name: ");
			fgets(enemyNameInput, sizeof(enemyNameInput), stdin);
			enemyNameInput[strlen(enemyNameInput) - 1] = '\0';	// discard the trailing '\n' char
			printf("\nPlease enter the enemy's Hp Stat: ");
			scanf("%d", &enemyHpInput);
			flushStdIn();
			printf("\nPlease enter the enemy's Attack Stat: ");
			scanf("%d", &enemyAttackInput);
			flushStdIn();
			addEnemy(mapNameInput, enemyNameInput, enemyHpInput, enemyAttackInput);
			printf("\nThe enemy was added! \n\n");
		}
		break;

	case 'l':					// list the maps an enemy shows up in
		tempList = list;
		displayMapEnemyList(tempList);
		break;
	
	case 'f':
		printf("\nPlease enter the map's name: ");
		fgets(mapNameInput, sizeof(mapNameInput), stdin);
		mapNameInput[strlen(mapNameInput) - 1] = '\0';	// dscanf("%d", &IDNumInput);iscard the trailing '\n' char

		if (searchMap(mapNameInput) == NULL)					
			printf("\nThe map does not exist or the list is empty! \n\n");
		else
		{
			combatOutcome = mapCombat(mapNameInput);
			if(combatOutcome == 0)
				printf("\nThe hero was slain! \n\n");
			else if(combatOutcome == 1)
				printf("\nThe hero was victorious! \n\n");
			else if(combatOutcome == 2)
				printf("\nThe hero is not currently in this map. \n \n");
			else
				printf("\nThis map has no enemies for the hero to fight! \n\n");
		}
		break;

	case 'q':					// quit
		break;

	default: printf("%c is invalid input!\n", c);
	}
}

// Displays the Global hero
void displayHero(){
	if(theHero == NULL)
		printf("\nThe Hero does not exist. \n");
	else{
		printf("\nThe Hero is here!");
		printf("\nThe Hero's name: %s", theHero->name);
		printf("\nThe Hero's current hp: %d", theHero->hp);
		printf("\nThe Hero's attack stat: %d", theHero->attack);
		printf("\nThe Hero's current location: %s\n", theHero->location);
	}
}

// Displays the maps in the mapList
void displayMapList(struct mapList* tempList)
{
	// write the code below.
	if (tempList == NULL)			// check if end of list is reached
	{
		printf("\n (end of list) \n");
		return;
	}
	else
	{
		printf("\nMap name: %s", tempList->map->name);
		printf("\nEnemy Count: %d", tempList->map->enemyCount);
		if(tempList->map->hero == NULL)
			printf("\nThere is no hero here. \n");
		else{
			printf("\n\nThere is a hero here.");
			printf("\nHero name: %s", tempList->map->hero->name);
			printf("\nHero current hp: %d", tempList->map->hero->hp);
			printf("\nHero attack stat: %d\n", tempList->map->hero->attack);
		}

		displayMapList(tempList->next);	// move on to next map
	}
}

// This function searches for all the maps that have a specific type of enemy present
// For example, if the enemy name was "goblin", it would search for all maps that have instances of
// "goblin" in their enemy list. It then prints the 
void displayMapEnemyList(struct mapList* tempList)
{
	// write the code below.scanf("%d", &IDNumInput);
	if (tempList == NULL)
	{
		printf("\nList is empty!\n");      	// check if list is empty
		return;
	}
	char enemyNameInput[MAX_NAME];
	printf("\nPlease enter enemy's name: ");
	fgets(enemyNameInput, sizeof(enemyNameInput), stdin);
	enemyNameInput[strlen(enemyNameInput) - 1] = '\0';
	int enemyPresent = 0;
	while (tempList != NULL) 			// traverse the list to display 'map' elements
	{
		struct enemy* enList = tempList->map->enemyList;
		if (enList != NULL)			// check if enemies are present in the map
		{
			int found = 0; 			// integer acting as a bool to check if the enemy was found in each map
			while (enList != NULL && found == 0)          // traverse the list of enemies to display
			{
				if (strcmp(enList->name, enemyNameInput) == 0)
				{
					printf("\nMap name: %s", tempList->map->name);
					printf("\nEnemy Count: %d", tempList->map->enemyCount);
					if(tempList->map->hero == NULL)
						printf("\nThere is no hero here. \n");
					else{
						printf("\n\nThere is a hero here.");
						printf("\nHero name: %s", tempList->map->hero->name);
						printf("\nHero current hp: %d", tempList->map->hero->hp);
						printf("\nHero attack stat: %d\n", tempList->map->hero->attack);
					}
					enemyPresent = 1;
					found = 1;
					break;
				}
				enList = enList->next;
			}
		}
		tempList = tempList->next;               // move on to the next map
	}
	if (enemyPresent == 0)
	{
		printf("\n There are no maps with the enemy %s present. \n", enemyNameInput);
	}
}


// HW7 Q1: addMap (15 points)
// This function is used to insert a new map into the linked list. 
// You must insert the new map to the head of linked list 'list'.
// You need NOT check if the map already exists in the list because that is taken care by searchMap() called in executeAction(). Look at how this function is used in executeAction().
// Don't bother trying to check how to implement searchMap() while implementing this function. Simply assume that the map does not exist in the list while implementing this function.
// NOTE: The function needs to add the map to the head of the list.
// NOTE: This function does not add a hero or enemies to the map. moveHero() in HW7 and addEnemy() in HW8 does each respectively.
// Hint: In this question, no enemies and no hero means hero = NULL and enemyList = NULL.
void addMap(char* mapNameInput, unsigned int enemyCount)
{
	struct map* newMap = (struct map*)malloc(sizeof(struct map));					// allocate memory for new map
	struct mapList* newMapNode = (struct mapList*)malloc(sizeof(struct mapList));	// allocate memory for new map node

	strcpy(newMap->name, mapNameInput);			// assign map name input to map name
	newMap->enemyCount = enemyCount;			// asign map enemy count input (0) to map enemy count

	newMap->hero = NULL;						// set hero and next node to NULL
	newMap->enemyList = NULL;

	newMapNode->map = newMap;					// assign map to the map list and insert node at the beginning
	newMapNode->next = list;
	list = newMapNode;
}

// HW7 Q2: searchMap (10 points)
// This function searches the 'list' to check if the given map exists in the list. Search by map name.
// If it exists then return that 'map' node of the list. Notice the return type of this function.
// If the map does not exist in the list, then return NULL.
// NOTE: After implementing this function, go to executeAction() to comment and un-comment the lines mentioned there which use searchMap()
//       in case 'a', case 'r', case 'l', case 'm', case 'c', case 'f' (total 6 places)
struct map* searchMap(char* mapNameInput)
{
	struct mapList* tempList = list;				// work on a copy of 'list'
	// Write your code below.

	while (tempList != NULL) {										// iterate through map list
		if (strcmp(tempList->map->name, mapNameInput) == 0) {		// if map name matches map name input return the map node of list
			return tempList->map;
		}
		tempList = tempList->next;
	}

	return NULL;								// return NULL if the map us not found
}


// HW7 Q3: createHero (5 points)
// This function creates a hero character and assigns it to the existing global hero: theHero. 
// If another hero already exists filling theHero, find where the hero currently is and remove them,
// the delete the hero and recreate them using user input. set the location to "Nowhere". Capitalization matters. 
// There is no need to check if the map name exists in the list. That is done in executeAction().
void createHero(char* heroNameInput, unsigned int heroHpInput, unsigned int heroAttackInput){
	// Write your code below.
	struct mapList* tempList = list;
	struct hero* newHero = malloc(sizeof(struct hero));
														// check if there is an existing hero and remove them 
	if (theHero != NULL) {
														// find the map where the hero is currently located
		while (tempList != NULL) {
			if (tempList->map->hero == theHero) {
				tempList->map->hero = NULL;
				break;
			}
			tempList = tempList->next;
		}																			// remove the old hero
		free(theHero);
	}																			// create hero

	strcpy(newHero->name, heroNameInput);
	newHero->hp = heroHpInput;
	newHero->attack = heroAttackInput;
	strcpy(newHero->location, "Nowhere");
																			// assign the new hero to theHero
	theHero = newHero;
}

// HW7 Q4: moveHero (10 points)
// This function moves theHero to another location on the map.
// Parse the list to locate the hero, have them "leave" the current map, and "travel" to another map.
// If the hero is not located in any map, simply have them "spawn" at the located map. 
// Update theHero's location as well.
// There is no need to check if the map name exists in the list. That is done in executeAction().
void moveHero(char* mapNameInput){
	struct mapList* tempList = list;		// create tempList to iterate
	// Write your code below.
	struct mapList* moveToList = list;

	strcpy(theHero->location, mapNameInput);

	while (tempList != NULL) {						// if the hero is not empty remove 
		if (tempList->map->hero != NULL) {
			tempList->map->hero = NULL;				// remove the hero from the map

			while (moveToList != NULL) {
				if (strcmp(moveToList->map->name, mapNameInput) == 0) {
					moveToList->map->hero = theHero;					// move to the hero to the new map 'travel'
					return;
				}
				moveToList = moveToList->next;
			}

			tempList->map->hero = theHero;		// 'spawn' hero at current map if new map not found
			return;
		}
		tempList = tempList->next;
	}

	moveToList = list;							// 'spawn' hero at the located map if they aren't found
	while (moveToList != NULL) {
		if (strcmp(moveToList->map->name, mapNameInput) == 0) {
			moveToList->map->hero = theHero;		// add hero to the map
			return;
		}
		moveToList = moveToList->next;
	}
}

// HW8 Q1: addEnemy (10 points)
// This function adds enemy's name to a map node.
// Parse the list to locate the map and add the enemy to that map's 'enemies' linked list. No need to check if the map name exists on the list. That is done in executeAction().
// If the 'enemies' list is empty, then add the enemy. If the map has existing enemies, then add the new enemy to the tail of the 'enemies' list.
// Multiples of the same named enemy can exist, and this is to be expected. You don't need to check for duplicates.
// For example, two goblins can exist in the 'enemies' list. 
void addEnemy(char* mapNameInput, char* enemyNameInput, unsigned int enemyHpInput, unsigned int enemyAttackInput)
{
	struct mapList* tempList = list;				// work on a copy of 'list'
	// write the code below.
	// create new 'enemy' node
	struct enemy* newEnemy = malloc(sizeof(struct enemy));		// allocate memory for enemy

	strcpy(newEnemy->name, enemyNameInput);			// copy input to the new enemy 
	newEnemy->hp = enemyHpInput;
	newEnemy->attack = enemyAttackInput;
	newEnemy->next = NULL;

	if (tempList->map->enemyList == NULL) {				// if the enemy list is NULL then add the new enemy to the head of enemy list
		tempList->map->enemyList = newEnemy;
	}
	else {
		struct enemy* currentEnemy = tempList->map->enemyList;		// set current enemy to head of enemy list
		while (currentEnemy->next != NULL) {
			currentEnemy = currentEnemy->next;					// iterate through to find spot for enemy
		}
		currentEnemy->next = newEnemy;							// place the new enemy in the list
	}
	enemyCounter++;												// increment global enemy count
	tempList->map->enemyCount = enemyCounter;
}

// HW8 Q2: removeMap (10 points)
// This function removes a map from the list.
// Parse the list to locate the map and delete that 'map' node.
// You need not check if the map exists because that is done in executeAction()
// The function will remove the hero on a local and global scope and the enemies of the map as well.
// When the map is located in the 'list', delete the hero, parse the 'enemies' list of that map,
// and remove the enemies.
// hint: deleting after deleting the hero on local, reset the global to base condition. 
void removeMap(char* mapNameInput)
{
	struct mapList* tempList = list;				// work on a copy of 'list'
	// write the code below.
	struct mapList* nextMap = NULL;					// set next instance of map to NULL

	struct enemy* currentEnemy = tempList->map->enemyList;			// set current enemy to head of enemy list
	while (currentEnemy != NULL) {
		struct enemy* nextEnemy = currentEnemy->next;				// 'next' enemy
		free(currentEnemy);											// free the current enemy
		currentEnemy = nextEnemy;									// go to next enemy
	}
	tempList->map->enemyList = NULL;					// set enemy list to NULL 'empty'
	enemyCounter = 0;									// set global enemy count to 0
	tempList->map->enemyCount = enemyCounter;

	free(tempList->map->hero);							// free the map hero
	tempList->map->hero = NULL;							// set the map hero to NULL
	theHero = NULL;										// set the global hero value to NULL

	while (tempList != NULL) {
		if (strcmp(tempList->map->name, mapNameInput) == 0) {			// if the map matches input
			if (nextMap == NULL) {
				list = tempList->next;									// set next node to list when nextMap is NULL
			}
			else {
				nextMap->next = tempList->next;							// set next node of list to next map 
			}
			free(tempList->map);										// free list map node
			free(tempList);												// free the map-list node
			return;
		}

		nextMap = tempList;												// set next map to list
		tempList = tempList->next;										// go to the next map
	}
}


// HW8 Q3 and Q4: mapCombat() and combatRecurse()  (20 points)
// These functions operate as a recursive pair. 
//
// mapCombat() works as the entry function into the recursion.
// Iterate through the list to find the map. You do not need to worry about if the map exists.
// executeAction() does this for you. Once you find the map, check if the hero is in the map. If they aren't,
// return 2 then check if there are enemies present on the map. If there aren't, return 3. 
// If both the hero and enemies are present, enter the combatRecurse() function to begin the fight.
// Upon exiting the recursion, if the returned integer is 0, update the map hero to NULL and then set
// the global theHero to NULL. Otherwise reset the enemyList to NULL and return 1. Recalculate the number of enemies after combat.
//
// combatRecurse() works as the recursive case for the fight. At each round of combat,
// take hp away from the hero equal to the attack stat of the current enemy and then take away
// hp from the the enemy equal to the attack stat of the hero. If the hero's health equals 0, delete the hero.
// If the enemy's health is 0, delete that enemy and move to the next enemy. Return 0 if the hero dies.
// Return 1 if all enemies die. In the case of a tie, return 0. 
int mapCombat(char* mapNameInput) {
	struct mapList* tempList = list;				// work on a copy of 'list'
	while (tempList != NULL) {
		if (strcmp(tempList->map->name, mapNameInput) == 0) {
			if (tempList->map->hero == NULL) {					// if the hero is not found return that
				return 2;
			}
			else if (tempList->map->enemyList == NULL) {		// if the enemy list is empty return that
				return 3;
			}
			else {
				int result = combatRecurse(tempList->map->hero, tempList->map->enemyList);
				if (result == 0) {								// if the hero dies remove them and reset the global hero variable
					tempList->map->hero = NULL;
					theHero = NULL;
				}
				else {
					tempList->map->enemyList = NULL;			// if all the enemies die set enemy list to NULL 'empty' and update enemy count to base value
					enemyCounter = 0;
					tempList->map->enemyCount = enemyCounter;
					return 1;
				}

				tempList->map->enemyCount = enemyCounter;		// update map enemy count
			}
		}
		tempList = tempList->next;								// go to next node
	}
	return 0;
}

int combatRecurse(struct hero* Hero, struct enemy* enemyList) {
	if (enemyList == NULL) {
		return 1;  // all enemies are defeated
	}

	Hero->hp -= enemyList->attack;			// take away hp from the hero equal to the attack damage of the enemy(s)
	enemyList->hp -= Hero->attack;			// vice versa

	if (Hero->hp <= 0) {
		free(Hero);
		return 0;  // hero is defeated
	}
	else if (enemyList->hp <= 0) {						// if the enemy dies
		struct enemy* nextEnemy = enemyList->next;
		free(enemyList);								// free the enemy
		enemyCounter--;									// decrement enemy count
		return combatRecurse(Hero, nextEnemy);  // attack next enemy
	}
	else if (Hero->hp <= 0 && enemyList->hp <= 0) {		// in the case of a tie return 0
		return 0;
	}
	else {
		return combatRecurse(Hero, enemyList);  // continue fighting current enemy
	}
}



