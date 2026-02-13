cmake_minimum_required(VERSION 3.23)
project(chess)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(chess_lib STATIC
  src/boards/board.cpp
  src/pieces/pawn.cpp
  src/boards/masks.cpp
)
target_include_directories(chess_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(chess src/main.cpp)
target_link_libraries(chess PRIVATE chess_lib)

enable_testing()
add_executable(chess_tests tests/test.pawn.cpp)
target_link_libraries(chess_tests PRIVATE chess_lib)
add_test(NAME generators COMMAND chess_tests)
