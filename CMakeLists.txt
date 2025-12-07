cmake_minimum_required(VERSION 3.23)
project(chess LANGUAGES CXX)
add_executable(chess)
target_sources(chess
  PRIVATE
    src/main.cpp
    src/boards/board.cpp
)
