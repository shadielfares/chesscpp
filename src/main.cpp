#include "boards/board.h"
#include "boards/masks.h"
#include "pieces/index.h"
#include "utils/utils.cpp"
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using std::string, std::vector, std::cin;

/*
 * @param from is 6-bit square
 * @param to is 6-bit square
 * @param flags is a 4-bit encoding that could be EN, Capture, Promotions, or
 * Castle
 */
int main() {
  uint64_t occupied_board = w_pieces | b_pieces;
  bool white_to_move = 1; // black_to_move = ~white_to_move = 0;
  uint64_t empty_board = ~occupied_board;
  uint64_t test_board = 0x800;
  vector<uint64_t> moves = pawn_move_generator(0x80000, test_board, white_to_move);
  print_board(moves.at(0));
  cout << endl;
  print_board(moves.at(1)); // valid double pawn moves
  return 0;
}

struct MakeMoveResponse {
  string from;
  string to;
};

class Player {
public:
  string from, to;

  MakeMoveResponse makeMove() {
    cout << "Enter the current position and target position: ";
    cin >> from >> to;
    return {from, to};
  }
};
