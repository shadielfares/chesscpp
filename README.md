# chess-

i want to build this funny chess game in c++, no ai used for this project at all, every single line will be written by me from scratch.

some libraries we're using:

someone already made a gui with that and honestly, making my own chess platform sounds pretty dope so i'm thinking of just making this a giga fast api
and then using one of the new frameworks for the client-side

will try aditya's cpp web-framework craft for this

Step 1. Mock out classes and class architecture, file architecture, create tickets.
Step 2. Pick up tickets related to backend architecture.
Step 3. ML-component in C++
Step 4. Client-side stuff

So I'm going to implement something similar, states to consider:
1. Start Game
2. Move:
    a. Select Piece
    b. Select Destination
3. Switch Player
4. Check Game Conditions
5. End Game

# Build

Oh also for some reason I forget that for compiling these cpp files use g++, and if I remember correctly it was gcc for just regular c files.. 

There is also this lore to the gcc people think its called the GNU C Compiler but I'm pretty sure it changed to GNU Compiler Collection because of this absolutely radical invention called the linker which is the reason why you can have language agnostic projects and use modules written in different programming languages in the same individual server or application.

Must have CMake installed, run `cmake --build build`

## Commands

- Build Libary
    `cmake -B build / cmake --build build`
- Run Tests
    `ctest --test-dir build`


## Cool Architecture Idea that I disregarded to collapse the complexity of this project

The idea is that you then have the engine implement inherit from this base class and implement the underlying functions but due to the underlying complexity I need to arrive at a working version fast and then add the abstractions

```
class CGameState {
public:
  void Init();
  void Move();
  void Switch();
  void Check();
  void End();
};

class CGameEngine {
public:
  void Init();
  void Move();
  void Switch();
  void Check();
  void End();

private:
  vector<CGameState *> states;
  bool m_running;
};
```
