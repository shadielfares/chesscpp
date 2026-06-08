#include "../boards/masks.h"
#include "index.h"
#include <cstdint>

Moves knight_move_generator(uint64_t knights, uint64_t occupied, uint64_t team_board) {
  uint64_t enemy_board = occupied & ~team_board;
  uint64_t quiet = 0;
  uint64_t captures = 0;

  uint64_t FILE_B = FILE_A << 1;
  uint64_t FILE_G = FILE_H >> 1;

  int shifts[8] = {17, 10, -6, -15, -17, -10, 6, 15}; // NNE, EEN, EES, SSE, SSW, WWS, WWN, NNW
  uint64_t wrap[8] = {~FILE_A, ~(FILE_A | FILE_B), ~(FILE_A | FILE_B), ~FILE_A,
                      ~FILE_H, ~(FILE_G | FILE_H), ~(FILE_G | FILE_H), ~FILE_H};

  for (int d = 0; d < 8; d++) {
    int s = shifts[d];
    uint64_t targets = (s > 0 ? knights << s : knights >> -s) & wrap[d];
    quiet |= targets & ~occupied;  // empty landing = move
    captures |= targets & enemy_board; // enemy landing = capture
  }
  return {quiet, captures};
}
