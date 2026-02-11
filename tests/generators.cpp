#include "boards/board.h"
#include "pieces/index.h"
#include "utils/utils.cpp"
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

bool _testSingleValidPawnMoves(uint64_t board) {
  uint64_t rank_3_pawns = 0xff0000;  // a3-h3
  return rank_3_pawns == board;
}

bool _testDoubleValidPawnMoves(uint64_t board) {
  uint64_t rank_4_pawns = 0xff000000;  // a4-h4
  return rank_4_pawns == board;
}

bool test_pawn_generator_from_game_start() {
  uint64_t board = w_pieces | b_pieces;
  vector<uint64_t> moves = pawn_move_generator(board, w_pieces, 1);
  return _testSingleValidPawnMoves(moves.at(0)) & _testDoubleValidPawnMoves(moves.at(1));
}

bool test_pawn_capture_generator(vector<uint64_t> valid_captures){
    uint64_t valid_left_capture = 0x1000000000;  // e5
    uint64_t valid_right_capture = 0x0;

    return valid_left_capture == valid_captures.at(0) & valid_right_capture == valid_captures.at(1);
}


bool test_pawn_capture(){
    uint64_t enemy_board = 0xef001000000000;  // black pieces on rank 7-8 + e5
    uint64_t white_pawns = 0x800ff00;         // rank 2 + d4
    bool white_to_move = 1;
    vector<uint64_t> valid_captures = pawn_capture_generator(enemy_board, white_pawns, white_to_move);
    bool res = test_pawn_capture_generator(valid_captures);
    return res;
}

bool test_black_pawn_move_blocked() {
    uint64_t black_pawns = 0x40000000000000;                  // g7
    uint64_t board = 0x40000000000000 | 0x400000000000;       // g7 + g6 (blocker)
    bool white_to_move = 0;
    vector<uint64_t> moves = pawn_move_generator(board, black_pawns, white_to_move);
    return moves.at(0) == 0x0 && moves.at(1) == 0x0;
}

bool test_black_pawn_capture() {
    uint64_t black_pawns = 0x40000000000000;   // g7
    uint64_t enemy_board = 0x800000000000;     // h6
    bool white_to_move = 0;
    vector<uint64_t> valid_captures = pawn_capture_generator(enemy_board, black_pawns, white_to_move);
    return valid_captures.at(0) == 0x0 && valid_captures.at(1) == 0x800000000000;  // valid right capture to h6
}

bool test_black_pawn_capture_hfile() {
    uint64_t black_pawns = 0x80000000000000; // h7
    uint64_t enemy_board = 0x1000000000000;  // a7
    bool white_to_move = 0;
    vector<uint64_t> valid_captures = pawn_capture_generator(enemy_board, black_pawns, white_to_move);
    return valid_captures.at(0) == 0x0 && valid_captures.at(1) == 0x0;  // h7 can only capture g6
}


//---

#define RUN_TEST(test_fn) (failures += !run_test(#test_fn, test_fn))

bool run_test(const char *name, bool (*test_fn)()) {
  bool result = test_fn();
  cout << name << ":" << (result ? "PASSED" : "FAILED") << endl;
  return result;
}

int main() {
  int failures = 0;

  RUN_TEST(test_pawn_generator_from_game_start);
  RUN_TEST(test_pawn_capture);
  RUN_TEST(test_black_pawn_move_blocked);
  RUN_TEST(test_black_pawn_capture);
  RUN_TEST(test_black_pawn_capture_hfile);

  return failures;
}
