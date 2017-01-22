#include <stdbool.h>
#include <string.h>

int gamePage;
int rightTilted = 0;

// Initializes the Orbit Oled screen
void GameUIInit()
{
    gamePage = 0;
    OrbitOledInit();
    OrbitOledClear();
    OrbitOledClearBuffer();
    OrbitOledSetFillPattern(OrbitOledGetStdPattern(iptnSolid));
    OrbitOledSetDrawMode(modOledSet);
}

/* Takes a pointer to the first element of the maze array as input
 * Gets called in UITick
 * Loops through the maze array of mazeEntry structs (defined in main file) 
 * and where it detects horizontal or vertical walls it draws them with the OrbitOled library funcitons
 * No return value
 */

static void makeMaze(mazeEntry * maze)
{
    int row; int col;
    
    for(int i = 1; i < 126; i++)
    {
        row = i / 21;
        col = i % 21;
        
        OrbitOledMoveTo(col*6, row*6);
        
        if((maze+i) -> hWall == 1)
        {
            for(int j = 0; j < 6; j++)
            {
                OrbitOledMoveTo(col*6 + j - 6, row*6);
                OrbitOledDrawPixel();
            }
        }
        
        if((maze+i) -> vWall == 1)
        {
            for(int j = 0; j < 6; j++)
            {
                OrbitOledMoveTo(col*6, row*6 + j - 6);
                OrbitOledDrawPixel();
            }
        }
    }   
  
    for(int i = 1; i< 4; i++)
    {
        for(int j = 1; j < 4; j++)
        {
            if((i+j) % 2 == 0)
            {
                OrbitOledMoveTo(115 + i, 25 + j);
                OrbitOledDrawPixel();
            }       
        }
    }
}

/* Takes direciton of movement (from tiltTick), 
 * the (x,y) coordinates of the space the ball wants to move to, 
 * and a pointer to the first element of the maze array
 * Looks at the desired direction of motion and the current coordinates
 * and scans to that point in the maze array to see if the ball will run into a wall with the movement
 * it returns 1 if the move is possible and 0 if it is not possible
 */
 
int checkMovement(char direction, int x, int y, mazeEntry *maze)
{
    int possible;
  
    if(x >= 0 && x <= 126 && y >= 0 && y <= 30)
    {
        switch(direction)
        {
            case 'U':
            {
                int check_x = x/6 + 1;
                int check_y = y/6;
                int i = ((check_y*21) + check_x);

                if(y % 6 != 0)
                {
                    possible = 1;
                }
                else if((maze+i) -> hWall == 0)
                {
                    possible = 1;
                }
                else
                {
                    possible = 0;
                }  
                break;
            }
  
            case 'D':
            {
                int check_x = x/6 + 1;
                int check_y = y/6;
                int i = ((check_y*21) + check_x); 

                if(y % 6 != 0)
                {
                    possible = 1;
                }
                else if((maze+i) -> hWall == 0)
                {
                    possible = 1;
                }
                else
                {
                    possible = 0;
                } 
                break;
            }
    
            case 'L':
            {
                int check_x = x/6;
                int check_y = y/6 + 1;
                int i = ((check_y*21) + check_x); 

                if(x % 6 != 0)
                {
                    possible = 1;
                }
                else if((maze+i) -> vWall == 0)
                {
                    possible = 1;
                }
                else
                {
                    possible = 0;
                }   
                break;
            }
      
            case 'R':
            {
                int check_x = x/6;
                int check_y = y/6 + 1;
                int i = ((check_y*21) + check_x);

                if(x % 6 != 0)
                {
                    possible = 1;
                }
                else if((maze+i) -> vWall == 0)
                {
                    possible = 1;
                }
                else
                {
                    possible = 0;
                }
                break;
            }
        }
    }
    
    else
    {
        possible = 0;
    }
    
    return possible;
}

/* Takes the desired direction and pointer to first element of maze array as input
 * has static variables x and y, these have to be static 
 * so that the coordinates of the ball are preserved between calls
 * it calls checkMovement, and if the movement is possible, it changes the coordinates 
 * of the ball according to the direction of movement
 * if the movement is impossible the ball's coordinates do not change
 * Finally, it redraws the ball with its updated (or not) coordinates
 * No return value
 */

void moveBall(char direction, mazeEntry *maze)
{
    static int x = 1;
    static int y = 1;
    int possible;

    switch(direction)
    {
        case 'U':
        {
            possible = checkMovement(direction,x,y+2, maze);
            
            if(possible)
            {
                y++;
            }
            break;
        }
            
        case 'D':
        {
            possible = checkMovement(direction,x,y-2, maze);
            
            if(possible)
            {
                y--;
            }
            break;
        }
        
        case 'L':
        {
            possible = checkMovement(direction,x-2,y, maze);
            
            if(possible)
            {
                x--;
            }
            break;
        }
        
        case 'R':
        {
            possible = checkMovement(direction,x+2,y, maze);
            
            if(possible)
            {
                x++;
            }
            break;
        }
        
        case 'N':
        {
            break;
        }
    }
  
    OrbitOledMoveTo(x,y);
    OrbitOledDrawPixel();
    OrbitOledMoveTo(x,y+1);
    OrbitOledDrawPixel();
    OrbitOledMoveTo(x+1,y);
    OrbitOledDrawPixel();
    OrbitOledMoveTo(x+1,y+1);
    OrbitOledDrawPixel();

    if((x >= 115 && x <= 119) && (y <= 30 && y >=26))
    {
        gamePage = 2;
    }
}


static void handlePageWelcome()
{
    OrbitOledMoveTo(35, 0);
    OrbitOledDrawString("Maze Game");
    OrbitOledMoveTo(5, 12);
    OrbitOledDrawString("Tilt right then");
    OrbitOledMoveTo(5, 24);
    OrbitOledDrawString("left to begin");
    char a = tiltTick();

    if(a == 'R')
    {
        rightTilted = 1;
    }

    if((a=='L') && rightTilted ==1)
    {
        gamePage=1;
        OrbitOledClearBuffer();
        OrbitOledClear();
    }
}



/* 
 *  Gets called in every loop
 *  Takes char a: direction of movement, and the maze as input
 *  Clears the orbit Oled screen of the last draw
 *  Controls the movement of the ball with moveball
 *  Draws the maze each time (there is no flickering if you order the commands correctly)
 *  Updates the Oled screen
 */ 
 
void GameUITick(char a, mazeEntry * maze)
{
    switch(gamePage)
    {
        case 0:
        {
            handlePageWelcome();
            OrbitOledUpdate();
            break;
        }

        case 1:
        {
            OrbitOledClearBuffer();
            makeMaze(maze);
            moveBall(a, maze);
            OrbitOledUpdate();
            break;
        }

        case 2:
        {
            OrbitOledClearBuffer();
            OrbitOledMoveTo(35, 10);
            OrbitOledDrawString("You Won!");
            OrbitOledUpdate();
            gamePage = 3;
            delay(3000);
            break;
        }

        case 3:
        {
            OrbitOledClearBuffer();
            OrbitOledMoveTo(2,0);
            OrbitOledDrawString("For a new maze");
            OrbitOledMoveTo(2, 10);
            OrbitOledDrawString("turn the board");
            OrbitOledMoveTo(2, 20);
            OrbitOledDrawString("off and on!");
            OrbitOledUpdate(); 
            break;
        }
    }
}