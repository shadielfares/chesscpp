#include "boards/board.h"
#include "pieces/index.h"
#include "utils/utils.cpp"
#include <cstdint>
#include <vector>

using std::vector;

bool _testSingleValidPawnMoves(uint64_t board) {
  uint64_t rank_3_pawns = 0xff0000; // a3-h3
  return rank_3_pawns == board;
}

bool _testDoubleValidPawnMoves(uint64_t board) {
  uint64_t rank_4_pawns = 0xff000000; // a4-h4
  return rank_4_pawns == board;
}

bool test_pawn_generator_from_game_start() {
  uint64_t board = w_pieces | b_pieces;
  vector<uint64_t> moves = pawn_move_generator(board, w_pieces, 1);
  return _testSingleValidPawnMoves(moves.at(0)) & _testDoubleValidPawnMoves(moves.at(1));
}

bool test_pawn_capture_generator(vector<uint64_t> valid_captures) {
  uint64_t valid_left_capture = 0x1000000000; // e5
  uint64_t valid_right_capture = 0x0;

  return valid_left_capture == valid_captures.at(0) & valid_right_capture == valid_captures.at(1);
}

bool test_pawn_capture() {
  uint64_t enemy_board = 0xef001000000000; // black pieces on rank 7-8 + e5
  uint64_t white_pawns = 0x800ff00;        // rank 2 + d4
  bool white_to_move = 1;
  vector<uint64_t> valid_captures = pawn_capture_generator(enemy_board, white_pawns, white_to_move);
  bool res = test_pawn_capture_generator(valid_captures);
  return res;
}

bool test_black_pawn_move_blocked() {
  uint64_t black_pawns = 0x40000000000000;            // g7
  uint64_t board = 0x40000000000000 | 0x400000000000; // g7 + g6 (blocker)
  bool white_to_move = 0;
  vector<uint64_t> moves = pawn_move_generator(board, black_pawns, white_to_move);
  return moves.at(0) == 0x0 && moves.at(1) == 0x0;
}

bool test_black_pawn_capture() {
  uint64_t black_pawns = 0x40000000000000; // g7
  uint64_t enemy_board = 0x800000000000;   // h6
  bool white_to_move = 0;
  vector<uint64_t> valid_captures = pawn_capture_generator(enemy_board, black_pawns, white_to_move);
  return valid_captures.at(0) == 0x0 && valid_captures.at(1) == 0x800000000000; // valid right capture to h6
}

bool test_black_pawn_capture_hfile() {
  uint64_t black_pawns = 0x80000000000000; // h7
  uint64_t enemy_board = 0x1000000000000;  // a7
  bool white_to_move = 0;
  vector<uint64_t> valid_captures = pawn_capture_generator(enemy_board, black_pawns, white_to_move);
  return valid_captures.at(0) == 0x0 && valid_captures.at(1) == 0x0; // h7 can only capture g6
}

bool test_en_passant() {
  uint64_t h_file_pawn = 0x1000000000000; // H7
  uint64_t h_file_pawn_twice_advanced = 0x100000000;
  uint64_t pawn_board = 0x200000000; // G5
  bool white_to_move = 1;            // yes

  uint64_t valid_en_passant_squre = 0x10000000000;
  vector<uint64_t> en_passant_moves = en_passant_generator(h_file_pawn, h_file_pawn_twice_advanced, pawn_board, white_to_move); // this should only be a valid right capture
  return en_passant_moves.at(1) == valid_en_passant_squre;
}

bool test_black_pawn_generator_from_game_start() {
  uint64_t board = w_pieces | b_pieces;
  vector<uint64_t> moves = pawn_move_generator(board, b_pawn_board, 0);
  uint64_t rank_6_pawns = 0xff0000000000; // a6-h6
  uint64_t rank_5_pawns = 0xff00000000;   // a5-h5
  return moves.at(0) == rank_6_pawns && moves.at(1) == rank_5_pawns;
}

bool test_black_pawn_single_move_d6() {
  uint64_t black_pawns = 0x8000000000000; // d7
  uint64_t board = black_pawns;
  vector<uint64_t> moves = pawn_move_generator(board, black_pawns, 0);
  return moves.at(0) == 0x80000000000; // d6
}

bool test_black_pawn_double_move_blocked_on_rank5() {
  uint64_t black_pawns = 0x10000000000000; // e7
  uint64_t blocker = 0x1000000000;         // e5
  uint64_t board = black_pawns | blocker;
  vector<uint64_t> moves = pawn_move_generator(board, black_pawns, 0);
  return moves.at(0) == 0x100000000000 && moves.at(1) == 0x0; // e6 single ok, double blocked
}

bool test_black_pawn_capture_both_sides() {
  uint64_t black_pawns = 0x8000000000000; // d7
  uint64_t enemy_board = 0x140000000000;  // c6 + e6
  vector<uint64_t> captures = pawn_capture_generator(enemy_board, black_pawns, 0);
  return captures.at(0) == 0x40000000000 && captures.at(1) == 0x100000000000; // c6 left, e6 right
}

bool test_black_pawn_capture_afile() {
  uint64_t black_pawns = 0x1000000000000;  // a7
  uint64_t enemy_board = 0x80000000000000; // h7 (should not wrap)
  vector<uint64_t> captures = pawn_capture_generator(enemy_board, black_pawns, 0);
  return captures.at(0) == 0x0 && captures.at(1) == 0x0;
}

bool test_black_en_passant() {
  uint64_t white_pawn_start = 0x200;        // b2
  uint64_t white_pawn_advanced = 0x2000000; // b4
  uint64_t black_pawns = 0x4000000;         // c4
  uint64_t valid_ep = 0x20000;              // b3
  vector<uint64_t> ep_moves = en_passant_generator(white_pawn_start, white_pawn_advanced, black_pawns, 0);
  print_board(ep_moves.at(0));
  return ep_moves.at(0) == valid_ep;
}

bool test_black_en_passant_hfile() {
  uint64_t white_pawn_start = 0x8000;        // h2
  uint64_t white_pawn_advanced = 0x80000000; // h4
  uint64_t black_pawns = 0x40000000;         // g4
  uint64_t valid_ep = 0x800000;              // h3
  vector<uint64_t> ep_moves = en_passant_generator(white_pawn_start, white_pawn_advanced, black_pawns, 0);
  return ep_moves.at(1) == valid_ep;
}

