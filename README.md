# SE-101-Project
# Language: C

Created this game along with my partner Samir Rehmtulla as our final project for our SE101 course. 
The game uses the accelerometer and OLED screen from an Orbit Booster pack and a Tiva Microcontroller.
The inspiration for the game came from the plastic mazes with the metal ball that we played as kids.
The game senses the tilt of the board from the user and moves the ball in that direction. The walls on the screen act like physical walls, with the ball stopping if it runs into them. The user must navigate the maze, and can generate a new game after completing a maze.
One of the most interesting aspects of the project is that each time the user creates a new game, a pseudo-random maze is generated, always with the same start and end points. The maze generator uses the onboard clock of the microcontroller for a random seed, and the recursive division algorithm for the maze.
