#include "boards/board.h"
#include "pieces/index.h"
#include "utils/utils.cpp"
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

bool _testSingleValidPawnMoves(uint64_t board) {
  uint64_t rank_3_pawns = 0xff0000;
  return rank_3_pawns == board;
}

bool _testDoubleValidPawnMoves(uint64_t board) {
  uint64_t rank_4_pawns = 0xff000000;
  return rank_4_pawns == board;
}

bool test_pawn_generator_from_game_start() {
  uint64_t board = w_pieces | b_pieces;
  vector<uint64_t> moves = pawn_move_generator(board, w_pieces, 1);
  return _testSingleValidPawnMoves(moves.at(0)) & _testDoubleValidPawnMoves(moves.at(1));
}

#define RUN_TEST(test_fn) run_test(#test_fn, test_fn)

void run_test(const char *name, bool (*test_fn)()) {
  bool result = test_fn();
  cout << name << ":" << (result ? "PASSED" : "FAILED") << endl;
}

int main() {
  RUN_TEST(test_pawn_generator_from_game_start);

  return 0;
}
