cmake_minimum_required(VERSION 3.23)
project(chess)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
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
  src/boards/bitboards.cpp
  src/pieces/pawn.cpp
  src/pieces/bishop.cpp
  src/pieces/rook.cpp
  src/pieces/queen.cpp
  src/pieces/knight.cpp
  src/pieces/king.cpp
  src/boards/masks.cpp
  src/boards/board_ops.cpp
  src/utils/utils.cpp
)
target_include_directories(chess_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(chess
  src/main.cpp
  src/renderer/gui.cpp
  src/renderer/geometry.cpp
  src/renderer/model.cpp
  src/renderer/controller.cpp
  src/renderer/view.cpp
  src/renderer/textures.cpp
)
target_link_libraries(chess PRIVATE chess_lib raylib)
target_compile_definitions(chess PRIVATE ASSETS_PATH="${CMAKE_SOURCE_DIR}/assets")
