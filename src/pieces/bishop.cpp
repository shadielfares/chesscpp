// the bishop moves along diagonals until a blocker stops the ray.
// we shift the whole bishop set one diagonal step at a time, masking file wraps.
#include "../boards/masks.h"
#include "index.h"
#include <cstdint>

// @param bishops the slider set to scan (also reused by the queen)
// @param occupied every piece on the board
// @param team_board friendly pieces (block the ray, can't be captured)
Moves bishop_move_generator(uint64_t bishops, uint64_t occupied,
                            uint64_t team_board) {
  uint64_t enemy_board = occupied & ~team_board;
  uint64_t quiet = 0;
  uint64_t captures = 0;

  int shifts[4] = {7, 9, -7, -9};                          // NW, NE, SE, SW
  uint64_t wrap[4] = {~FILE_H, ~FILE_A, ~FILE_A, ~FILE_H}; // drop edge wraps

  for (int d = 0; d < 4; d++) {
    int s = shifts[d];
    uint64_t ray = bishops;
    while (ray) {
      ray = (s > 0 ? ray << s : ray >> -s) & wrap[d]; // one step, no wrap
      quiet |= ray & ~occupied;                       // empty square reachable
      captures |= ray & enemy_board;                  // enemy on it = capture
      ray &= ~occupied;                               // stop at any blocker
    }
  }
  return {quiet, captures};
}
