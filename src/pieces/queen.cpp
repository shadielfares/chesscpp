// the queen is a bishop and a rook in one: union of both ray sets.
#include "../boards/board.h"
#include "index.h"
#include <cstdint>

// @param occupied every piece on the board
// @param team_board friendly pieces (block the ray, can't be captured)
// @param white_to_move picks which queen set to scan
Moves queen_move_generator(uint64_t occupied, uint64_t team_board,
                           bool white_to_move) {
  uint64_t queen = white_to_move ? w_queen_board : b_queen_board;

  Moves bishop = bishop_move_generator(queen, occupied, team_board);
  Moves rook = rook_move_generator(queen, occupied, team_board);

  return {bishop.quiet | rook.quiet, bishop.captures | rook.captures};
}

