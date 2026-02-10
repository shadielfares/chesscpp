#include "../boards/masks.h"
#include <cstdint>
#include <vector>
using namespace std;
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

struct MoveInfo {
  // I believe this is a 16 bit uint_64 that accounts for all the move types of
  // all pieces
};

#define RANK_4 0xff000000ULL;
#define RANK_5 0xff00000000ULL;

/*
 * @param uint64_t board is the occupied board of all pieces
 * @param uint64_t pawn_board
 * @param bool white_to_move playing side ( 0 | 1 ), 1 is true
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
