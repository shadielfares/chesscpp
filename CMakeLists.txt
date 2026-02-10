cmake_minimum_required(VERSION 3.23)
project(chess) 
set(CMAKE_CXX_STANDARD 20)
add_executable(chess)
target_sources(chess
  PRIVATE
    src/main.cpp
    src/boards/board.cpp
    src/pieces/pawn.cpp
)
