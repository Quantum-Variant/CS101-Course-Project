This is a helper document that lists the code files included and the features that have been implemented in the project.

In this ZIP file, please find the following included:
-> bubble.h: A header file used to implement the properties and behaviour of the bubbles
-> bullet.h: A header file used to implement the properties and behaviour of the bullets
-> main.cpp: The main code file that calls the header files to implement the functionality of the game
-> shooter.h: A header file used to implement the properties and behaviour of the shooter

The features that have been implemented in the game are as follows:
-> Required Features:
   1) The bubble movement has been modified to incorporate vertical motion such that the bubbles travel in a parabolic path. When the ball hits the horizontal ground or strikes the upper boundary, it rebounds and progresses along the new path.
   2) Collisions between the bubble and the bullet have been introduced; the bubble and bullet disappear after they collide. 
   3) Collisions between the bubble and the shooter have been introduced; the shooter loses a life if a bubble hits it.
-> Extra Features:
   1) The score, a timer and a life counter have been introduced. The score cumulatively increases whenever you hit a bubble, depending on how much time is left (If the bubble is hit within the first ten seconds, the score increases by 500 points; if a bubble is hit within the next ten seconds, the score increases by 400 points, and so on). The game gets over when the time runs out or if the shooter loses all three lives (the shooter loses a life when hit by a bubble).
   2) Three levels have been implemented in the game. Each level becomes progressively difficult in terms of more bubbles and less time. The bubbles in different levels have different colors.

Link to the folder of screen recordings of the Graphics Window, demonstrating the features implemented:
https://drive.google.com/drive/folders/1saFn_za0_PwGmTHAGBNQYt9s51vTrXzn?usp=sharing