cmake_minimum_required(VERSION 3.23)
project(chess)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
  raylib
  GIT_REPOSITORY https://github.com/raysan5/raylib.git
  GIT_TAG 5.5
)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(raylib)

add_library(chess_lib STATIC
  src/boards/board.cpp
  src/pieces/pawn.cpp
  src/pieces/bishop.cpp
  src/pieces/rook.cpp
  src/pieces/queen.cpp
  src/pieces/knight.cpp
  src/pieces/king.cpp
  src/boards/masks.cpp
  src/boards/board_ops.cpp
)
target_include_directories(chess_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(chess src/main.cpp)
target_link_libraries(chess PRIVATE chess_lib)

add_executable(chess_gui src/renderer/gui.cpp)
target_link_libraries(chess_gui PRIVATE chess_lib raylib)
target_compile_definitions(chess_gui PRIVATE ASSETS_PATH="${CMAKE_SOURCE_DIR}/assets")

enable_testing()
add_executable(chess_tests tests/test.pawn.cpp)
target_link_libraries(chess_tests PRIVATE chess_lib)
add_test(NAME generators COMMAND chess_tests)
