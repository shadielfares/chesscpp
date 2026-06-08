// the rook moves along ranks & files until a blocker stops the ray.
// we shift the whole rook set one step at a time, masking file wraps.
#include "../boards/masks.h"
#include "index.h"
#include <cstdint>

// @param rooks the slider set to scan (also reused by the queen)
// @param occupied every piece on the board
// @param team_board friendly pieces (block the ray, can't be captured)
Moves rook_move_generator(uint64_t rooks, uint64_t occupied,
                          uint64_t team_board) {
  uint64_t enemy_board = occupied & ~team_board;
  uint64_t quiet = 0;
  uint64_t captures = 0;

  int shifts[4] = {8, -8, 1, -1};                      // N, S, E, W
  uint64_t wrap[4] = {~0ULL, ~0ULL, ~FILE_A, ~FILE_H}; // verticals never wrap

  for (int d = 0; d < 4; d++) {
    int s = shifts[d];
    uint64_t ray = rooks;
    while (ray) {
      ray = (s > 0 ? ray << s : ray >> -s) & wrap[d]; // one step, no wrap
      quiet |= ray & ~occupied;                       // empty square reachable
      captures |= ray & enemy_board;                  // enemy on it = capture
      ray &= ~occupied;                               // stop at any blocker
    }
  }
  return {quiet, captures};
}
