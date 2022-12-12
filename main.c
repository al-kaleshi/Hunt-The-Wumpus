
/* ---------------------------------------------
Prog 2: Wumpus (bats, arrow, and all arrays are dynamic)

Course: CS 211, Fall 2022. Wednesday 4pm lab
System: Mac using Replit
Author: Ali Kaleshi
---------------------------------------------
*/

// These are all the libraries I used to complete this program.
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Here I define the constant values to help me build and initiate the game.
#define kNumBats 2
#define kNumPits 2
#define kNumObjects 7
#define kNumRooms 20
#define kNumAdjRooms 3

// The Game
struct Game {
  int **rooms;
  int wumpusRoom;
  int *batRooms;
  int *pitRooms;
  int arrowRoom;
  int playerRoom;
  bool isRunning;
  int moveCount;
};

//--------------------------------------------------------------------------------
// Function displayCave() receives no paramaters and returns nothing because it
// is void. The function prints the game board to the terminal.
void displayCave() {
  printf("\n"
         "       ______18______             \n"
         "      /      |       \\           \n"
         "     /      _9__      \\          \n"
         "    /      /    \\      \\        \n"
         "   /      /      \\      \\       \n"
         "  17     8        10     19       \n"
         "  | \\   / \\      /  \\   / |    \n"
         "  |  \\ /   \\    /    \\ /  |    \n"
         "  |   7     1---2     11  |       \n"
         "  |   |    /     \\    |   |      \n"
         "  |   6----5     3---12   |       \n"
         "  |   |     \\   /     |   |      \n"
         "  |   \\       4      /    |      \n"
         "  |    \\      |     /     |      \n"
         "  \\     15---14---13     /       \n"
         "   \\   /            \\   /       \n"
         "    \\ /              \\ /        \n"
         "    16---------------20           \n"
         "\n");
}

//--------------------------------------------------------------------------------
// Function diplayInstructions() receives no paramaters and returns nothing
// because it is void. The function prints the games instructions to the
// terminal.
void displayInstructions() {
  printf("Hunt the Wumpus:                                             \n"
         "The Wumpus lives in a completely dark cave of 20 rooms. Each \n"
         "room has 3 tunnels leading to other rooms.                   \n"
         "                                                             \n"
         "Hazards:                                                     \n"
         "1. Two rooms have bottomless pits in them.  If you go there you fall "
         "and die.   \n"
         "2. Two other rooms have super-bats.  If you go there, the bats grab "
         "you and     \n"
         "   fly you to some random room, which could be troublesome.  Then "
         "those bats go \n"
         "   to a new room different from where they came from and from the "
         "other bats.   \n"
         "3. The Wumpus is not bothered by the pits or bats, as he has sucker "
         "feet and    \n"
         "   is too heavy for bats to lift.  Usually he is asleep.  Two things "
         "wake       \n"
         "    him up: Anytime you shoot an arrow, or you entering his room.  "
         "The Wumpus   \n"
         "    will move into the lowest-numbered adjacent room anytime you "
         "shoot an arrow.\n"
         "    When you move into the Wumpus' room, then he wakes and moves if "
         "he is in an \n"
         "    odd-numbered room, but stays still otherwise.  After that, if he "
         "is in your \n"
         "    room, he snaps your neck and you die!                            "
         "           \n"
         "                                                                     "
         "           \n"
         "Moves:                                                               "
         "           \n"
         "On each move you can do the following, where input can be upper or "
         "lower-case:  \n"
         "1. Move into an adjacent room.  To move enter 'M' followed by a "
         "space and       \n"
         "   then a room number.                                               "
         "           \n"
         "2. Shoot your guided arrow through a list of up to three adjacent "
         "rooms, which  \n"
         "   you specify.  Your arrow ends up in the final room.               "
         "           \n"
         "   To shoot enter 'S' followed by the number of rooms (1..3), and "
         "then the      \n"
         "   list of the desired number (up to 3) of adjacent room numbers, "
         "separated     \n"
         "   by spaces. If an arrow can't go a direction because there is no "
         "connecting   \n"
         "   tunnel, it ricochets and moves to the lowest-numbered adjacent "
         "room and      \n"
         "   continues doing this until it has traveled the designated number "
         "of rooms.   \n"
         "   If the arrow hits the Wumpus, you win! If the arrow hits you, you "
         "lose. You  \n"
         "   automatically pick up the arrow if you go through a room with the "
         "arrow in   \n"
         "   it.                                                               "
         "           \n"
         "3. Enter 'R' to reset the person and hazard locations, useful for "
         "testing.      \n"
         "4. Enter 'C' to cheat and display current board positions.           "
         "           \n"
         "5. Enter 'D' to display this set of instructions.                    "
         "           \n"
         "6. Enter 'P' to print the maze room layout.                          "
         "           \n"
         "7. Enter 'X' to exit the game.                                       "
         "           \n"
         "                                                                     "
         "           \n"
         "Good luck!                                                           "
         "           \n"
         " \n\n");
} // end displayInstructions()

// Function generateRandomInt(int min, int max) receivies two ints min
// and max as paramaters and returns a random int. This function is used to
// help randomize board locations for the wumpus, pits, and player.
int generateRandomInt(int min, int max) {
  return (rand() % (max - min + 1)) + min;
}

// Function initUniqueList(int list[], int length, int min, int max)
// recieves an array list, int min, and int max and parameters. The function
// takes these parameters to then initializse and assign values into a unique
// list with no repeated numbers. This function is void and does not return
// anything.
void initUniqueList(int list[], int length, int min, int max) {
  int size = 0;
  while (size != length) {
    int value = generateRandomInt(min, max);
    bool found = false;

    for (int index = 0; index < size; index++) {
      if (list[index] == value) {
        found = true;
        break;
      }
    }

    if (!found) {
      list[size] = value;
      size++;
    }
  }
}

// Function **createRooms() creates the dynamic room for the cave structure and
// initalizes all the values, return an array which is the cave structure (int
// array[][]).
int **createRooms() {
  int **rooms = (int **)malloc(sizeof(int[kNumRooms][kNumAdjRooms]));
  for (int i = 0; i < kNumRooms; i++) {
    rooms[i] = (int *)malloc(sizeof(int[kNumAdjRooms]));
  }
  rooms[0][0] = 1, rooms[0][1] = 4, rooms[0][2] = 7;
  rooms[1][0] = 0, rooms[1][1] = 2, rooms[1][2] = 9;
  rooms[2][0] = 1, rooms[2][1] = 3, rooms[2][2] = 11;
  rooms[3][0] = 2, rooms[3][1] = 4, rooms[3][2] = 13;
  rooms[4][0] = 0, rooms[4][1] = 3, rooms[4][2] = 5;
  rooms[5][0] = 4, rooms[5][1] = 6, rooms[5][2] = 14;
  rooms[6][0] = 5, rooms[6][1] = 7, rooms[6][2] = 16;
  rooms[7][0] = 0, rooms[7][1] = 6, rooms[7][2] = 8;
  rooms[8][0] = 7, rooms[8][1] = 9, rooms[8][2] = 17;
  rooms[9][0] = 1, rooms[9][1] = 8, rooms[9][2] = 10;
  rooms[10][0] = 9, rooms[10][1] = 11, rooms[10][2] = 18;
  rooms[11][0] = 2, rooms[11][1] = 10, rooms[11][2] = 12;
  rooms[12][0] = 11, rooms[12][1] = 13, rooms[12][2] = 19;
  rooms[13][0] = 3, rooms[13][1] = 12, rooms[13][2] = 14;
  rooms[14][0] = 5, rooms[14][1] = 13, rooms[14][2] = 15;
  rooms[15][0] = 14, rooms[15][1] = 16, rooms[15][2] = 19;
  rooms[16][0] = 6, rooms[16][1] = 15, rooms[16][2] = 17;
  rooms[17][0] = 8, rooms[17][1] = 16, rooms[17][2] = 18;
  rooms[18][0] = 10, rooms[18][1] = 17, rooms[18][2] = 19;
  rooms[19][0] = 12, rooms[19][1] = 15, rooms[19][2] = 18;
  return rooms;
}

// Function destroyRooms(int **rooms) that takes in an int **rooms and frees the
// memory, does not return anything because it is void.
void destroyRooms(int **rooms) {
  for (int i = 0; i < kNumRooms; i++) {
    free(rooms[i]);
  }
  free(rooms);
}

// Function *createBatRooms() that takes in nothing and allocates enough space
// for the bats array and then it returns the int array.
int *createBatRooms() {
  int *batRoom = (int *)malloc(sizeof(int) * kNumBats);
  return batRoom;
}

// Function *createPitRooms() that takes in nothing and allocates enough space
// for the pits array and then it returns the int array.
int *createPitRooms() {
  int *pitRoom = (int *)malloc(sizeof(int) * kNumPits);
  return pitRoom;
}

// Function initGame(struct Game *g) takes in a pointer to a Game struct as a
// paramter. It then goes through to initalize and assign values to the
// objectPositions array, taking into account that you need to subtract one to
// prcoess the correct room. This function is void and does not return anything.
void initGame(struct Game *g) {
  int *objectPositions = (int *)malloc(sizeof(int) * kNumObjects);
  initUniqueList(objectPositions, kNumObjects, 1, kNumRooms);
  for (int index = 0; index < kNumObjects; index++) {
    objectPositions[index]--;
  }
  g->rooms = createRooms();
  g->batRooms = createBatRooms();
  g->pitRooms = createPitRooms();
  g->moveCount = 0;
  g->playerRoom = objectPositions[0];
  g->wumpusRoom = objectPositions[1];
  g->pitRooms[0] = objectPositions[2];
  g->pitRooms[1] = objectPositions[3];
  g->batRooms[0] = objectPositions[4];
  g->batRooms[1] = objectPositions[5];
  g->arrowRoom = objectPositions[6];
  g->isRunning = true;
}

// Function isValidRoom(int room) takes in an int as a paramter and compares it
// to make sure that the room number is in the bounds of the array and is
// actually a valid room number. Returns a boolean based on if the room is in
// the bounds or not.
bool isValidRoom(int room) { return (room >= 0) && (room < kNumRooms); }

// Function isAdjRoom(int room, int adjRoom) takes in two ints as paramters.
// The first int being the current room and the second int being another room
// that is going to be checked to confirm whether or not it is adjacent. The
// function returns a boolean true or false based on whether or not the room is
// adjacent.
bool isAdjRoom(struct Game *g, int room, int adjRoom) {
  for (int index = 0; index < kNumAdjRooms; index++) {
    if (g->rooms[room][index] == adjRoom) {
      return true;
    }
  }
  return false;
}

// Function processHazards(struct Game *g) takes in a pointer to a struct Game
// as a parameter. The function then goes to check if the adjacent rooms are
// equal to either the wumpus or pit rooms. If the function finds one of the
// cases to be true it will print a message to the console. The function returns
// nothing because it is void.
void processHazards(struct Game *g) {
  if (isAdjRoom(g, g->playerRoom, g->wumpusRoom)) {
    printf("You smell a stench. ");
  }

  if (isAdjRoom(g, g->playerRoom, g->pitRooms[0]) ||
      isAdjRoom(g, g->playerRoom, g->pitRooms[1])) {
    printf("You feel a draft. ");
  }

  if (isAdjRoom(g, g->playerRoom, g->batRooms[0]) ||
      isAdjRoom(g, g->playerRoom, g->batRooms[1])) {
    printf("You hear rustling of bat wings. ");
  }

  printf("\n");
}

// Function resetGame(struct Game *g, int positions[]) takes in a pointer to a
// struct Game and an array of poistions as parameters. The function the
// reassigns the player, wumpus, and pitRooms to the passed in array poisitons
// values. This function is void and returns nothing.
void resetGame(struct Game *g, int positions[]) {
  g->playerRoom = positions[0];
  g->wumpusRoom = positions[1];
  g->pitRooms[0] = positions[2];
  g->pitRooms[1] = positions[3];
  g->batRooms[0] = positions[4];
  g->batRooms[1] = positions[5];
  g->arrowRoom = positions[6];
}

// Function movePlayer(struct Game *g, int room) takes in a pointer to a
// struct Game and an int room as parameters. The function then reassigns the
// player room to the room passed in. This function is void and does not return
// anything.
void movePlayer(struct Game *g, int room) { g->playerRoom = room; }

// Function processReset(struct Game *g) takes in a pointer to a
// struct Game as a parameter. The funciton reads input from the user and then
// takes into acount the offset. After that it then passes the changes to the
// positions array that was initiazed at the start of this function into
// resetGame(struct Game *g, int positions[]). This function processReset(struct
// Game *g) is void and does not return anything.
void processReset(struct Game *g) {
  int *positions = (int *)malloc(sizeof(int) * kNumObjects);
  for (int index = 0; index < kNumObjects; index++) {
    positions[index] = -1;
  }
  scanf(" %d %d %d %d %d %d %d", &positions[0], &positions[1], &positions[2],
        &positions[3], &positions[4], &positions[5], &positions[6]);
  for (int index = 0; index < kNumObjects; index++) {
    positions[index]--;
  }

  if (positions[6] == -2) {
    positions[6] = -1;
  }

  resetGame(g, positions);
  free(positions);
}

// Function batMove(struct Game *g, int room) takes in a pointer to a
// struct Game. The function then does the logic to determines where the bat
// should drop the person if he/she is found. This function does not return
// anything because it is void.
void batMove(struct Game *g) {
  int batIndex = g->playerRoom == g->batRooms[0] ? 0 : 1;
  int otherBatRoom =
      g->playerRoom == g->batRooms[0] ? g->batRooms[1] : g->batRooms[0];
  int *positions = (int *)malloc(sizeof(int) * 4);
  positions[0] = g->playerRoom + 1;
  positions[1] = otherBatRoom + 1;
  positions[2] = -1;
  positions[3] = -1;
  int size = 2;

  while (size != 4) {
    int value = generateRandomInt(1, 20);
    bool found = false;
    for (int index = 0; index < size; index++) {
      if (positions[index] == value) {
        found = true;
        break;
      }
    }

    if (!found) {
      positions[size] = value;
      size++;
    }
  }
  for (int index = 0; index < 3; index++) {
    positions[index] -= 1;
  }
  g->playerRoom = positions[2];
  g->batRooms[batIndex] = positions[3];
  free(positions);
}

// Function processMove(struct Game *g) takes in a pointer to a
// struct Game as a parameter. The funciton reads input from the user and then
// takes into acount the offset. Next, it checks if the room meets the critera
// for the player to be moved into it / all the hazards and arro room. Lastly,
// it checks for if you move into a hazard room and based on which one it prints
// an output to the console of the result of the move. This function returns a
// boolean value based on if the move could be processed or not.
bool processMove(struct Game *g) {
  int room = -1;
  scanf(" %d", &room);
  room--;

  if (!isValidRoom(room) || !isAdjRoom(g, g->playerRoom, room)) {
    return false;
  }

  movePlayer(g, room);

  if (g->playerRoom == g->wumpusRoom && g->wumpusRoom % 2 == 0) {
    g->wumpusRoom = g->rooms[g->wumpusRoom][0];
    printf("You hear a slithering sound, as the Wumpus slips away. \n"
           "Whew, that was close! \n");
  } else if (g->playerRoom == g->wumpusRoom && g->wumpusRoom % 2 != 0) {
    printf("You briefly feel a slimy tentacled arm as your neck is snapped.\n"
           "It is over.\n");
    g->isRunning = false;
  } else if (g->pitRooms[0] == g->playerRoom ||
             g->pitRooms[1] == g->playerRoom) {
    printf("Aaaaaaaaahhhhhh....   \n");
    printf("    You fall into a pit and die. \n");
    g->isRunning = false;
  } else if (g->batRooms[0] == g->playerRoom ||
             g->batRooms[1] == g->playerRoom) {
    batMove(g);
    printf("Woah... you're flying!\n");
    printf("You've just been transported by bats to room %d.\n",
           g->playerRoom + 1);
  }
  if (g->playerRoom == g->arrowRoom) {
    printf("Congratulations, you found the arrow and can once again shoot. \n");
    g->arrowRoom = -1;
  }
  return true;
}

// Function processGuess(struct Game *g) takes in a pointer to a
// struct Game as a parameter. The function reads in user input and then takes
// into account the offset. After that it checks if you guessed the wumpus room
// correctly. Based on your guess it will print if you won or lost.  This
// function is void and returns nothing.
void processGuess(struct Game *g) {
  int room = -1;
  scanf(" %d", &room);
  room--;

  if (room == g->wumpusRoom) {
    printf("You won!\n");
  } else if (room != g->wumpusRoom) {
    printf("You lost.\n");
  }
  g->isRunning = false;
}

// Function shoot(struct Game *g, int positions[], int length) takes in a
// pointer to a struct Game, positions array, and length. The function then does
// the logic to determines if/where the arrow ends up after a shoot command.
// Also the function checks if it ricochets and if it does it does the
// corresponding logic. This function returns a boolean.
bool shoot(struct Game *g, int positions[], int length) {
  bool ricochet = false;
  int currentRoom = g->playerRoom;
  int index = 0;
  while (index != length) {
    int nextRoom = positions[index];
    if (isAdjRoom(g, currentRoom, nextRoom)) {
      currentRoom = nextRoom;
    } else {
      printf("Room %d is not adjacent.  Arrow ricochets...\n", nextRoom + 1);
      ricochet = true;
      currentRoom = g->rooms[currentRoom][0];
      break;
    }
    index++;
  }

  g->arrowRoom = currentRoom;

  if (g->arrowRoom == g->wumpusRoom) {
    g->isRunning = false;
  } else {
    g->wumpusRoom = g->rooms[g->wumpusRoom][0];
  }

  if (g->arrowRoom == g->playerRoom) {
    g->isRunning = false;
  }

  return ricochet;
}

// Function processShot(struct Game *g) takes in a pointer to a
// struct Game as a parameter. It then reads the user input into an array and
// does the logic for determining if it should call the shoot function. After
// this it prints what happneded post shot. This function is void and returns
// nothing.
void processShot(struct Game *g) {
  int *positions = (int *)malloc(sizeof(int) * 3);
  for (int index = 0; index < 3; index++) {
    positions[index] = -1;
  }
  int length = -1;

  if (g->arrowRoom != -1) {
    printf("Sorry, you can't shoot an arrow you don't have.  Go find it.\n");
    return;
  }

  printf("Enter the number of rooms (1..3) into which you want to shoot, "
         "followed by the rooms themselves: ");
  scanf("%d", &length);

  if (length > 3) {
    printf("Sorry, the max number of rooms is 3.  Setting that value to 3.");
    length = 3;
  }

  if (length == 1) {
    scanf("%d", &positions[0]);
  } else if (length == 2) {
    scanf("%d %d", &positions[0], &positions[1]);
  } else {
    scanf("%d %d %d", &positions[0], &positions[1], &positions[2]);
  }

  for (int index = 0; index < length; index++) {
    positions[index]--;
  }

  bool ricochet = shoot(g, positions, length);

  if (g->isRunning == false && g->wumpusRoom == g->arrowRoom && !ricochet) {
    printf("Wumpus has just been pierced by your deadly arrow! \n"
           "Congratulations on your victory, you awesome hunter you.\n");
    return;
  }

  if (g->isRunning == false && g->playerRoom == g->arrowRoom && ricochet) {
    printf("You just shot yourself, and are dying.\n"
           "It didn't take long, you're dead.\n");
    return;
  }

  if (g->isRunning == false && g->arrowRoom == g->wumpusRoom && ricochet) {
    printf("Your arrow ricochet killed the Wumpus, you lucky dog!\n"
           "Accidental victory, but still you win!\n");
    return;
  }

  if (g->isRunning == false && g->playerRoom == g->arrowRoom && !ricochet) {
    printf("You just shot yourself.  \n"
           "Maybe Darwin was right.  You're dead.\n");
    return;
  }
  free(positions);
}

// Function processExit(struct Game *g) takes in a pointer to a
// struct Game as a parameter. It then sets the g->isRunning option to false.
// This function is void and returns nothing.
void processExit(struct Game *g) { g->isRunning = false; }

// Function processCommand(struct Game *g) takes in a pointer to a
// struct Game as a parameter. This function is where all the logic stems out
// from. It will print the room and hazards first always then based on the case
// you entered it will complete the command and count it as a move if it is a
// valid command. Lastly, if it is an invalid command the program will tell you.
// This function is void and returns nothing.
void processCommand(struct Game *g) {
  printf("You are in room %d. ", g->playerRoom + 1);
  processHazards(g);

  bool retry = false;
  do {

    printf("\n%d. Enter your move (or 'D' for directions): ", g->moveCount + 1);
    char command = '\0';
    scanf(" %c", &command);

    switch (toupper(command)) {
    case 'R':
      printf("Enter the room locations (1..20) for player, wumpus, pit1, "
             "pit2, bats1, bats2, and arrow: "
             "\n\n");
      processReset(g);
      retry = false;
      break;
    case 'C':
      printf("Cheating! Game elements are in the following rooms: \n"
             "Player Wumpus Pit1 Pit2 Bats1 Bats2 Arrow \n"
             "%4d %7d %5d %4d %4d %4d %5d\n\n",
             g->playerRoom + 1, g->wumpusRoom + 1, g->pitRooms[0] + 1,
             g->pitRooms[1] + 1, g->batRooms[0] + 1, g->batRooms[1] + 1,
             g->arrowRoom == -1 ? -1 : g->arrowRoom + 1);
      retry = false;
      break;
    case 'M':
      if ((retry = !processMove(g))) {
        printf("Invalid move.  Please retry. \n");
        printf("You are in room %d. ", g->playerRoom + 1);
        processHazards(g);
        break;
      }
      g->moveCount++;
      break;
    case 'D':
      displayCave();
      displayInstructions();
      retry = false;
      break;
    case 'P':
      displayCave();
      retry = false;
      break;
    case 'X':
      retry = false;
      g->isRunning = false;
      break;
    case 'G':
      printf("Enter room (1..20) you think Wumpus is in: ");
      processGuess(g);
      retry = false;
      break;
    case 'S':
      processShot(g);
      retry = false;
      g->moveCount++;
      break;
    default:
      retry = true;
      printf("Invalid move.  Please retry. \n");
      printf("You are in room %d. ", g->playerRoom + 1);
      processHazards(g);
    }
  } while (retry);
}

// Main - This function is where the game is declared and initalized. Also this
// is where the game always comes back to process the command entered by the
// user. The function will return an int if the function
// was succesful(0) or not(1) at the end of the game.
int main(void) {
  srand(time(0));
  srand(1);

  struct Game g;
  initGame(&g);

  while (g.isRunning) {
    processCommand(&g);
  }
  free(g.batRooms);
  free(g.pitRooms);
  destroyRooms(g.rooms);
  printf("\nExiting Program ...\n");

  return EXIT_SUCCESS;
}