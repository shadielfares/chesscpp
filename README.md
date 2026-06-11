# chess-

https://github.com/user-attachments/assets/f7d280ee-7275-4694-ace5-479dc3fe8de6

## disclaimer: use of ai in this project

this project was a passionate project of mine and i wanted to maximize learning value. so here's the honest breakdown of what's mine vs ai assisted.

within the renderer/ package, other than the usage of raylib and the MVC pattern and finding the chess sprites, much of that code was ai written, except any external masks or bitboards which i hand wrote.

anything within board/ and pieces/ was 80-90% hand written, with the initial strategy developed by myself and the occasional code cleanup by an LLM.

additionally, whenever I was super lost I would ask GPT for some help but no code snippets were copied from its generation.

i want to build this funny chess game in c++, no ai used for this project at all, every single line will be written by me from scratch.

some libraries we're using:

raylib for the native gui, real piece textures and mouse drag-to-move, pure c++ with no js/embind.

someone already made a gui with that and honestly, making my own chess platform sounds pretty dope so i'm thinking of just making this a giga fast api
and then using one of the new frameworks for the client-side

# Build

Oh also for some reason I forget that for compiling these cpp files use g++, and if I remember correctly it was gcc for just regular c files.. 

There is also this lore to the gcc people think its called the GNU C Compiler but I'm pretty sure it changed to GNU Compiler Collection because of this absolutely radical invention called the linker which is the reason why you can have language agnostic projects and use modules written in different programming languages in the same individual server or application.

Must have CMake installed, run `cmake --build build`

## Commands

- Build Library
    `cmake -B build / cmake --build build`
- Run Tests
    `ctest --test-dir build`
    `cmake -B build; ./build/chess_tests`

