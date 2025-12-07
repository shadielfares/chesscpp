# chess-

i want to build this funny chess game in c++, no ai used for this project at all, every single line will be written by me from scratch.

some libraries we're using:

Client Side UI: SFML
Server-Side: i dont think i'll use a framework for rn

Step 1. Mock out classes and class architecture, file architecture, create tickets.
Step 2. Pick up tickets related to backend architecture.
Step 3. ML-component in C++
Step 4. Client-side stuff

--- 
Read this cool article on managing game states and game engine:
http://gamedevgeek.com/tutorials/managing-game-states-in-c/

So I'm going to implement something similar, states to consider:
1. Start Game
2. Move:
    a. Select Piece
    b. Select Destination
3. Switch Player
4. Check Game Conditions
5. End Game

00000000  00000000
00000000  00000000
00000000  00000000
00000000  00000000
00000000  00000000
00000000  00100000
00000000  00000000
01000000  00000000
