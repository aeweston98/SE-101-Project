#include <delay.h>
#include <FillPat.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
    int hWall;
    int vWall;
}mazeEntry;

/* A structure used to represent each maze coordinate. 
 *  Each intersection space in 5x20 size grid for the maze is represented by a mazeEntry.
 *  Column outside the left border and the row above the top border are also represented by mazeEntrys
 *  hWall can be '1' or '0'
 *  Represents if the intersection has a horizontal wall (drawn below the intersection)
 *  vWall can be '1' or '0'
 *  Represents if the intersection has a vertical wall (drawn to the right of the intersection)
 */

mazeEntry * drawMaze();
int blankMaze(mazeEntry maze[6][21]);
int initMaze(mazeEntry maze[6][21]);
int drawTopBottomWall(mazeEntry maze[6][21]);
int drawLeftRightWall(mazeEntry maze[6][21]);
int divide(mazeEntry maze[6][21], int leftBound, int rightBound, int upBound, int lowBound);
int drawColumnRow(mazeEntry maze[6][21], int leftBound, int rightBound, int upBound, int lowBound, int column, int row);
int removeTopVertical(mazeEntry maze[6][21], int upBound, int lowBound, int column);
int removeBottomVertical(mazeEntry maze[6][21], int upBound, int lowBound, int column);
int removeLeftHorizontal(mazeEntry maze[6][21], int leftBound, int rightBound, int row);
int removeRightHorizontal(mazeEntry maze[6][21], int leftBound, int rightBound, int row);

void WireInit();
void GameUIInit();
void tiltInit();

char tiltTick();
void GameUITick();

mazeEntry *maze = drawMaze();

 /*
  * Returns an array of mazeEntrys as a pointer after it has been constructed
  * This array contains a mazeEntry for each intersection space of the maze
  * Each mazeEntry indicates if the intersection should have a horizontal and/or vertical wall drawn.
  */

mazeEntry * drawMaze()
{
    mazeEntry maze[6][21];
    initMaze(maze);
    mazeEntry *fin_maze = (mazeEntry*) malloc(sizeof(mazeEntry)*6*21);
    memcpy(fin_maze, maze, sizeof(mazeEntry)*6*21);    
    return fin_maze;
}

/*
 * Initializes the maze
 * Calls blankMaze to set all the mazeEntry hWall and vWall members to 0
 * Calls drawTopBottomWall to set the top and bottom boundaries of the maze
 * Calls drawLeftRightWall to set the left and right boundaries of the maze
 * Calls divide to initially divide the maze into 4 chambers
 */

int initMaze(mazeEntry maze[6][21])
{
    blankMaze(maze);
    drawTopBottomWall(maze);
    drawLeftRightWall(maze);
    divide(maze, 1,19, 1, 4);
    return 0;
}

/*
 * Clears the maze (Sets each mazeEntry's vWall and hWall members to 0)
 */

int blankMaze(mazeEntry maze[6][21])
{
    for(int i = 0; i<6; i++)
    {
        for(int j=0; j<21; j++)
        {
            maze[i][j].hWall = 0;
            maze[i][j].vWall = 0;
        }
    }
}

/*
 * Sets the top border of the maze and bottom border of the maze
 * Accomplishes this by accessing the mazeEntrys that belong to the top and bottom rows (0 and 5)
 * Sets their hWall members to 1
 */

int drawTopBottomWall(mazeEntry maze[6][21])
{
    int j=1;
    while(j<21)
    {
       maze[0][j].hWall = 1;
       maze[5][j].hWall = 1;
       j++;
    }
}

/*
 * Sets the left and right borders of the maze
 * Accomplishes this by accessing the mazeEntrys that belong to the left and right columns (0 and 20)
 * Sets their vWall members to 1.
 */

int drawLeftRightWall(mazeEntry maze[6][21])
{
    for(int i = 1; i<6; i++)
    {
        maze[i][0].vWall = 1;
        maze[i][20].vWall = 1;
    }
    return 0;
}

/*
 * Function receives top, bottom, right and left boundaries of a chamber.
 * Bisects the maze vertically at a random index between the left and right bounds
 * Bisects the maze horizontally at a random index between the upper and lower bounds
 * Results in 4 smaller chambers
 */
 
int divide(mazeEntry maze[6][21], int leftBound, int rightBound, int upBound, int lowBound)
{
    randomSeed(analogRead(A8));
    int hDistance = rightBound - leftBound;
    int column = (random(0,1000)%(hDistance+1))+leftBound;
    
    int vDistance = lowBound - upBound;
    int row = (random(0,1000)%(vDistance+1)) + upBound;
    drawColumnRow(maze, leftBound, rightBound, upBound, lowBound, column, row);    
}

/*
 * This function receives boundaries from divide, as well as indices that indicate where to draw a row and column
 * It sets a column and row at the indices passed as row and column 
 * Accomplishes this by setting the vWall members to 1 down a column of mazeEntrys
 * Or the hWall members to 1 across a row
 * The row and column set form a cross in the chamber bounded by leftBound, rightBound, upBound, lowBound
 * The cross can be considered as 4 walls. This function randomly chooses 3 of the 4 walls to draw a slit in
 * It then recursively calls divide on the 4 subchambers formed (provided they have not reached the minimum size)
 */

int drawColumnRow(mazeEntry maze[6][21], int leftBound, int rightBound, int upBound, int lowBound, int column, int row)
{
    for(int i = upBound; i< (lowBound + 2); i++)
    {
        maze[i][column].vWall = 1;
    }
    for(int j=leftBound; j<(rightBound+2); j++)
    {
        maze[row][j].hWall = 1;
    }
    
    randomSeed(analogRead(A7));
    
    int rando = (random(0,1000) % 4);
    
    switch(rando)
    {
        case 0:
        {
            removeTopVertical(maze, upBound, row, column);
            removeBottomVertical(maze, row, lowBound, column);
            removeLeftHorizontal(maze, leftBound, column, row);
            break;
        }
        
        case 1:
        {
            removeTopVertical(maze, upBound, row, column);
            removeBottomVertical(maze, row, lowBound, column);
            removeRightHorizontal(maze, column, rightBound, row);
            break;
        }
        
        case 2:
        {
            removeTopVertical(maze, upBound, row, column);
            removeLeftHorizontal(maze, leftBound, column, row);
            removeRightHorizontal(maze, column, rightBound, row);
            break;
        }
    
        case 3:
        {
            removeBottomVertical(maze, row, lowBound, column);
            removeLeftHorizontal(maze, leftBound, column, row);
            removeRightHorizontal(maze, column, rightBound, row);
            break;
        }
    }
    
    if(column!=leftBound && row!=upBound)
    {
        divide(maze, leftBound, column-1, upBound, row-1);
    }
    
    if(column!=rightBound && row!=upBound)
    {
        divide(maze, column+1, rightBound, upBound, row-1);
    }
    
    if(column!=leftBound && row!=lowBound)
    {
        divide(maze, leftBound, column-1, row+1, lowBound);
    }
    
    if(column!=rightBound && row!=lowBound)
    {
        divide(maze, column+1, rightBound, row+1, lowBound);
    } 
}

/*
 * divide and drawColumnRow divided a chamber with a vertical line and horizontal line
 * This function puts a slit in the top portion of the cross formed by the vertical and horizontal lines
 * This occurs at a random index in the top vertical portion.
 */

int removeTopVertical(mazeEntry maze[6][21], int upBound, int lowBound, int column)
{
    int range = lowBound - upBound + 1;
    randomSeed(analogRead(A3));
    int remove = (random(0,1000) % range) + upBound;
    maze[remove][column].vWall = 0;
}

/*
 * divide and drawColumnRow divided a chamber with a vertical line and horizontal line
 * This function puts a slit in the lower portion of the cross formed by the vertical and horizontal lines
 * This occurs at a random index in the lower vertical portion.
 */

int removeBottomVertical(mazeEntry maze[6][21], int upBound, int lowBound, int column)
{
    int range = lowBound - upBound + 1;
    randomSeed(analogRead(A4));
    int remove = (random(0,1000) % range) + upBound + 1;
    maze[remove][column].vWall = 0;
}

/*
 * divide and drawColumnRow divided a chamber with a vertical line and horizontal line
 * This function puts a slit in the left portion of the cross formed by the vertical and horizontal lines
 * This occurs at a random index in the left horizontal portion.
 */

int removeLeftHorizontal(mazeEntry maze[6][21], int leftBound, int rightBound, int row)
{
    int range = rightBound - leftBound + 1;
    randomSeed(analogRead(A5));
    int remove = (random(0,1000) % range) + leftBound;
    maze[row][remove].hWall = 0;
}

/*
 * divide and drawColumnRow divided a chamber with a vertical line and horizontal line
 * This function puts a slit in the right portion of the cross formed by the vertical and horizontal lines
 * This occurs at a random index in the right horizontal portion.
 */

int removeRightHorizontal(mazeEntry maze[6][21], int leftBound, int rightBound, int row)
{
    int range = rightBound - leftBound + 1;
    randomSeed(analogRead(A6));
    int remove = (random(0,1000) % range) + leftBound+1;
    maze[row][remove].hWall = 0;
}

void setup() 
{
    WireInit();
    Serial.begin(9600);

    delay(100);
    randomSeed(A9);
    
    tiltInit();
    GameUIInit();
}

void loop() 
{ 
    char a = tiltTick();
    GameUITick(a, maze);
}