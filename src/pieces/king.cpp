#include "../boards/masks.h"
#include "../boards/squares.h"
#include "index.h"
#include <cstdint>

namespace {
const uint64_t SHORT_PATH_WHITE = bit(F1) | bit(G1);
const uint64_t LONG_PATH_WHITE = bit(B1) | bit(C1) | bit(D1);
const uint64_t SHORT_PATH_BLACK = bit(F8) | bit(G8);
const uint64_t LONG_PATH_BLACK = bit(B8) | bit(C8) | bit(D8);
} // namespace

// @param king the king bitboard
// @param occupied every piece on the board
// @param team_board friendly pieces (can't land on them)
Moves king_move_generator(uint64_t king, uint64_t occupied, uint64_t team_board) {
  uint64_t enemy_board = occupied & ~team_board;
  uint64_t quiet = 0;
  uint64_t captures = 0;

  int shifts[8] = {8, -8, 1, -1, 9, 7, -7, -9}; // N, S, E, W, NE, NW, SE, SW
  uint64_t wrap[8] = {~0ULL,   ~0ULL,   ~FILE_A, ~FILE_H,
                      ~FILE_A, ~FILE_H, ~FILE_A, ~FILE_H};

  for (int d = 0; d < 8; d++) {
    int s = shifts[d];
    uint64_t targets = (s > 0 ? king << s : king >> -s) & wrap[d];
    quiet |= targets & ~occupied;
    captures |= targets & enemy_board;
  }
  return {quiet, captures};
}

// castle target squares (king lands on g-file short, c-file long).
// checks only rights + empty path. TODO: forbid castling through/into check
// once square-attack detection exists.
// @param can_short / can_long castling rights (king + that rook never moved)
uint64_t castling_moves(uint64_t occupied, bool white_to_move, bool can_short,
                        bool can_long) {
  uint64_t targets = 0;

  if (white_to_move) {
    if (can_short && !(occupied & SHORT_PATH_WHITE)) targets |= bit(G1);
    if (can_long && !(occupied & LONG_PATH_WHITE)) targets |= bit(C1);
  } else {
    if (can_short && !(occupied & SHORT_PATH_BLACK)) targets |= bit(G8);
    if (can_long && !(occupied & LONG_PATH_BLACK)) targets |= bit(C8);
  }
  return targets;
}
