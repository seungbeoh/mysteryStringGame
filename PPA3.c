// Preprocessors
#include <stdio.h>
#include <stdlib.h>
#include "MacUILib.h"
#include "myStringLib.h"   
#include <time.h>
#define SIZE 12

// Global Variables
int exitFlag;
int gameWon;
int found;
int goalStrLen;
char input; 
int i, j, k;

// Global Pointers
const char goalStr[] = "McMaster-ECE"; 
char *mysteryString;
int moveCount;
char charCandidate;

struct objPos
{
    int x;
    int y;
    char symbol;
};

struct objPos player = {10, 5, '@'};
struct objPos *itemBin;

enum
{
    LEFT, // 0
    RIGHT, // 1
    UP, // 2
    DOWN, // 3
    STOP // 4
}dir; 

enum
{
    slow1 = 500000, // 0
    slow2 = 250000, // 1
    normal = 100000, // 2
    fast1 = 50000, // 3
    fast2 = 10000 // 4
}speed = normal;


void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);
void GenerateItems(struct objPos list[], const int listSize, const struct objPos *playerObj, const int xRange, const int yRange, const char* str);

// Main Program
int main(void)
{

    Initialize();

    while(!exitFlag)  
    {
        GetInput();

        RunLogic();

        DrawScreen();

        LoopDelay();
    }

    CleanUp();

    return 0;
}

// Initialization Routine
void Initialize(void)
{
    MacUILib_init();

    MacUILib_clearScreen();

    // Variable Initialization
    input = 0; 
    exitFlag = 0;  
    moveCount = 0;
    found = 0;
    dir = STOP; 
    gameWon = 0;
    
    // Allocate DMA
    mysteryString = malloc((SIZE + 1) * sizeof(char));
    itemBin = malloc(5 * sizeof(struct objPos)); 

    // Logic to reveal mystery string 
    for(k = 0; k < SIZE; k++)
    {
        mysteryString[k] = '?';
    }
    mysteryString[SIZE] = '\0';

    // Generating random food items 
    srand(time(NULL)); 
    GenerateItems(itemBin, 5, &player, 19, 9, goalStr);
}


// Input collection routine, asynchronous input 
void GetInput(void)
{
    if (MacUILib_hasChar())
    {
        input = MacUILib_getChar();
    }
}


// Main Logic Routine
void RunLogic(void)
{
    if(input != 0)  
    {
        
        switch(input)
        { 
            case 27:  
                exitFlag = 1;
                break;

            case 'W': 
            case 'w':
                if ((dir == LEFT) || (dir == RIGHT) || (dir == STOP))
                    {
                        moveCount++;
                        dir = UP;
                        break;
                    }
                
                else
                    break;

            case 'A':
            case 'a':
                if ((dir == UP) || (dir == DOWN) || (dir == STOP)) 
                    {
                        moveCount++;
                        dir = LEFT;
                        break;
                    }
                
                else
                    break;

            case 'S':
            case 's':
                if ((dir == LEFT) || (dir == RIGHT) || (dir == STOP)) 
                    {
                        moveCount++;
                        dir = DOWN;
                        break;
                    }
                
                else
                    break;

            case 'D':
            case 'd':
                    if ((dir == UP) || (dir == DOWN) || (dir == STOP)) 
                    {
                        moveCount++;
                        dir = RIGHT;
                        break;
                    }
                
                else
                    break;

            case '1': 
                speed = slow1;
                break;

            case '2':
                speed = slow2;
                break;
            
            case '3':
                speed = normal;
                break;
            
            case '4':
                speed = fast1;
                break;

            case '5':
                speed = fast2;
                break;

            default:
                break;

        }

        input = 0;
    }

    if (dir == LEFT)        
    {
        --player.x;
    }

    else if (dir == RIGHT)        
    {
        ++player.x;
    }

    else if(dir == UP)
    {
        --player.y;
    }

    else if (dir == DOWN)
    {
        ++player.y;
    }

    // Border wrap around for player 
    if(player.x == 19) 
    {
        player.x = 1;
    }

    else if(player.x == 0)
    {
        player.x = 18;
    }

    else if(player.y == 0)
    {
        player.y = 8;
    }

    else if(player.y == 9)
    {
        player.y = 1;
    }

    // Object collision logic 
    for(i = 0; i < 5; i++)  
    {
        if((player.x == itemBin[i].x) && (player.y == itemBin[i].y)) 
        {   
            for(j = 0; j < SIZE ; j++) 
                if(goalStr[j] == itemBin[i].symbol) 
                    mysteryString[j] = itemBin[i].symbol; 
            
            GenerateItems(itemBin, 5, &player, 19, 9, goalStr); 
        }
    }

    // Game winning logic 
    if (my_strcmp(mysteryString, goalStr) == 1) 
    {
        gameWon = 1;
        exitFlag = 1;
    }
}


// Draw Routine
void DrawScreen(void) 
{
    MacUILib_clearScreen();

    for (i = 0; i <= 9; i++)
    {
        for(j = 0; j <= 19; j++)
        {   
            found = 0;

            for(k = 0; k < 5; k++)
            {
                if((j == itemBin[k].x) && (i == itemBin[k].y))
                {
                    found = 1;
                    break;
                }
            }

            if (((i == 0) || (i == 9)) || ((j == 0) || (j == 19)))
            {
                MacUILib_printf("#");
            }

            else if(found != 0) 
            {
                MacUILib_printf("%c", itemBin[k].symbol);
            }

            else if ((j == player.x) && (i == player.y)) 
                {
                    MacUILib_printf("%c", player.symbol);
                }

            else  
            {
                MacUILib_printf(" ");
            } 

            if (j == 19)
            {
                MacUILib_printf("\n");
            }
        }
    }    

    // Display mystery string to help player stay on track 
    MacUILib_printf("Mystery String: %s\n", mysteryString);
    MacUILib_printf("Move Count: %d\n\n", moveCount);  

    // Debugging Messages
    MacUILib_printf("====Debugging Messages====\n");
    for(i = 0; i < 5; i++)
    {
        MacUILib_printf("Character Locations: %d, %d, %c\n", itemBin[i].x, itemBin[i].y, itemBin[i].symbol);
    }
    MacUILib_printf("Player Location: %d, %d\n", player.x, player.y);
    
    if(exitFlag == 1)
    {
        if(gameWon == 1)
            MacUILib_printf("Congratulations! You found all the characters and won the game!\n");
        else
            MacUILib_printf("Game over. You exited the game early.\n");
    }
}

// Delay Routine
void LoopDelay(void)
{
    MacUILib_Delay(speed); 
}


// Tear-Down Routine
void CleanUp(void)
{
    free(mysteryString);
    free(itemBin);
    MacUILib_uninit();
}


// Item Generation Routine 
void GenerateItems(struct objPos list[], const int listSize, const struct objPos *playerObj, const int xRange, const int yRange, const char* str)
{
    int xCandidate;
    int yCandidate;
    int randCharIndex;
    int isValid;
    
    goalStrLen = my_strlen(str); 

    // Generate 5 Items
    for(i = 0; i < listSize; i++) 
    {
        do {   
            isValid = 1;

            // Generate a candidate coordinate
            xCandidate = (rand() % (xRange - 2)) + 1; // rand() % 17 + 1 -> range is 1 to 17. 
            yCandidate = (rand() % (yRange - 2)) + 1; // rand() % 7 + 1 -> range is 1 to 7.

            // First 2 items from itemBin are from goalStr and last 3 are randomized from ASCII range
            if(i < 2)
            {
                randCharIndex = rand() % goalStrLen;
                charCandidate = str[randCharIndex];
            }

            else 
            {
                charCandidate = (rand() % (126 - 33 + 1)) + 33;
                
                if(charCandidate == '@' || charCandidate == '#' || charCandidate == ' ')
                    isValid = 0;
            }

            // Check against duplicate coordinates 
            if (xCandidate == playerObj->x && yCandidate == playerObj->y) 
            {
                isValid = 0;
            }

            // Check against previous position and characters
            for (j = 0; j < i; j++) 
            {
                // Check coordinate collision
                if (list[j].x == xCandidate && list[j].y == yCandidate) 
                {
                    isValid = 0;
                    break;
                }

                if (list[j].symbol == charCandidate) 
                {
                    isValid = 0;
                    break;
                }
            }

        } while(isValid == 0); // Check failed, discard value

        // Check succeeded, assign validated candidate values to bin item (struct)
        list[i].x = xCandidate;
        list[i].y = yCandidate;
        list[i].symbol = charCandidate;
    }
}

