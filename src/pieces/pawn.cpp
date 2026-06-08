#include "../boards/masks.h"
#include "index.h"
#include <cstdint>
#include <vector>
using std::vector;

/*
 * For Pawns you only have the following valid move types:
 * 1. Advance 1 Tile
 * 2. Advance 2 Tile
 * 3. En Passant
 * 4. Capture
 *
 * Further extended you only have 2 more special move types if you consider all
 * piece types:
 *
 * 5. Castles (Short and Long)
 * 6. Promotion (Flag)
 */

#define RANK_4 0xff000000ULL;
#define RANK_5 0xff00000000ULL;

/*
 * @param uint64_t board is the occupied board of all pieces
 * @param uint64_t pawn_board (black pawn_board or white pawn_board)
 * @param bool white_to_move playing side ( 0 | 1 ), 1 is true
 * @returns {single_advance_moves, double_advance_moves}
 */
vector<uint64_t> pawn_move_generator(uint64_t board, uint64_t pawn_board,
                                     bool white_to_move) {
  uint64_t single_advance_moves, double_advance_moves;

  if (white_to_move) {
    // Rules for a single pawn move forward
    // 1. There cannot be an allied piece on the target square.
    // 2. There cannot be an enemy piece on the target square.
    single_advance_moves =
        (pawn_board << 8) & ~board; // we need the shifted board ANDED with the
                                    // empty version of the current board
    // Rules for a double pawn move forward
    // 1. There cannot be an allied piece on the target square AND there cannot
    // be an allied or enemy piece on the first square preceiding the second
    // one.
    // 2. There cannot be an enemy piece on the target square.
    double_advance_moves = (single_advance_moves << 8) & ~board & RANK_4;
  } else {
    single_advance_moves = (pawn_board >> 8) & ~board;
    double_advance_moves = (single_advance_moves >> 8) & ~board & RANK_5;
  }
  return {single_advance_moves, double_advance_moves};
};

vector<uint64_t> pawn_capture_generator(uint64_t enemy_board, uint64_t pawn_board, bool white_to_move) {
  uint64_t valid_left_captures, valid_right_captures;
  if (white_to_move) {
    valid_right_captures = ((pawn_board & ~FILE_A) << 7) & enemy_board;
    valid_left_captures = ((pawn_board & ~FILE_H) << 9) & enemy_board;
  } else {
    valid_right_captures = ((pawn_board & ~FILE_H) >> 7) & enemy_board;
    valid_left_captures = ((pawn_board & ~FILE_A) >> 9) & enemy_board;
  }
  return {valid_left_captures, valid_right_captures};
}

vector<uint64_t> en_passant_generator(uint64_t last_enemy_pawns, uint64_t current_enemy_pawns, uint64_t pawn_board, bool white_to_move) {
  // ok something we can try is we only pass in en_passant valid moves and then check perform an & operation with pawns on RANK_4 for black and RANK_5 for white
  // going to assume that we are tracking every single board and are storing a vector of all possible positions
  uint64_t valid_left_en_passant_captures, valid_right_en_passant_captures;
  if (white_to_move) {
    uint64_t pawns_started_on_rank_7 = last_enemy_pawns & RANK_7;
    uint64_t landed_on_rank_5 = current_enemy_pawns & RANK_5;
    uint64_t double_moved = (pawns_started_on_rank_7 >> 16) & landed_on_rank_5;

    uint64_t my_pawns_on_rank_5 = pawn_board & RANK_5;
    uint64_t adjacent_left = (my_pawns_on_rank_5 & ~FILE_H) << 1;
    uint64_t adjacent_right = (my_pawns_on_rank_5 & ~FILE_A) >> 1;

    valid_left_en_passant_captures = (double_moved & adjacent_left) << 8;
    valid_right_en_passant_captures = (double_moved & adjacent_right) << 8;
  } else {
    uint64_t pawns_started_on_rank_2 = last_enemy_pawns & RANK_2;
    uint64_t landed_on_rank_4 = current_enemy_pawns & RANK_4;
    uint64_t double_moved = (pawns_started_on_rank_2 << 16) & landed_on_rank_4;

    uint64_t my_pawns_on_rank_4 = pawn_board & RANK_4;
    uint64_t adjacent_left = (my_pawns_on_rank_4 & ~FILE_A) >> 1;
    uint64_t adjacent_right = (my_pawns_on_rank_4 & ~FILE_H) << 1;

    valid_left_en_passant_captures = (double_moved & adjacent_left) >> 8;
    valid_right_en_passant_captures = (double_moved & adjacent_right) >> 8;
  }
  return {valid_left_en_passant_captures, valid_right_en_passant_captures};
}

// packs the three pawn generators into one PawnMoves struct.
// pass 0 for the en-passant history when there's no prior move to consider.
PawnMoves pawn_moves(uint64_t occupied, uint64_t enemy_board,
                     uint64_t last_enemy_pawns, uint64_t current_enemy_pawns,
                     uint64_t pawn_board, bool white_to_move) {
  vector<uint64_t> advance = pawn_move_generator(occupied, pawn_board, white_to_move);
  vector<uint64_t> capture = pawn_capture_generator(enemy_board, pawn_board, white_to_move);
  vector<uint64_t> ep = en_passant_generator(last_enemy_pawns, current_enemy_pawns, pawn_board, white_to_move);

  return {advance.at(0), advance.at(1), capture.at(0),
          capture.at(1), ep.at(0),      ep.at(1)};
}
