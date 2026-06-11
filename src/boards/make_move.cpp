#include "boards/board_ops.h"
#include "boards/bitboards.h"
#include "boards/position.h"
#include "boards/squares.h"
#include "move.h"
#include <array>
#include <cctype>

using std::array;

// en passant removes the pawn that was passed; any other capture clears the
// enemy piece sitting on the target square
static void apply_capture(CMove move, int to, bool white,
                          array<PieceBoard, 12> &boards) {
  if (move.get_flag() == FLAG_EP_CAPTURE) {
    uint64_t captured = bit(white ? to - BOARD_SIZE : to + BOARD_SIZE);
    (white ? b_pawn_board : w_pawn_board) &= ~captured;
  } else if (move.is_capture()) {
    for (PieceBoard &entry : boards)
      if (entry.white != white) *entry.bits &= ~bit(to);
  }
}

// the board a promotion flag's piece letter resolves to, for the moving side
static uint64_t *promotion_board(char piece, bool white) {
  switch (piece) {
  case 'N': return white ? &w_knight_board : &b_knight_board;
  case 'B': return white ? &w_bishop_board : &b_bishop_board;
  case 'R': return white ? &w_rook_board : &b_rook_board;
  default:  return white ? &w_queen_board : &b_queen_board;
  }
}

// hop the rook over the king it just castled past
static void move_castle_rook(int flag, bool white) {
  if (flag == FLAG_CASTLE_SHORT) {
    if (white) w_rook_board = (w_rook_board & ~bit(H1)) | bit(F1);
    else       b_rook_board = (b_rook_board & ~bit(H8)) | bit(F8);
  } else if (flag == FLAG_CASTLE_LONG) {
    if (white) w_rook_board = (w_rook_board & ~bit(A1)) | bit(D1);
    else       b_rook_board = (b_rook_board & ~bit(A8)) | bit(D8);
  }
}

// a king/rook leaving home, or a home rook being captured, forfeits rights
static void update_castling_rights(int from, int to, char type, bool white) {
  if (type == 'K') {
    if (white) rights.white_short = rights.white_long = false;
    else rights.black_short = rights.black_long = false;
  }
  if (from == A1 || to == A1) rights.white_long = false;
  if (from == H1 || to == H1) rights.white_short = false;
  if (from == A8 || to == A8) rights.black_long = false;
  if (from == H8 || to == H8) rights.black_short = false;
}

// applies a move for the side to move. the flag carries the move's semantics
// (capture, en passant, castle, double push, promotion) so nothing is
// re-derived here.
bool make_move(CMove move) {
  int from = move.get_from(), to = move.get_to(), flag = move.get_flag();
  if (!valid_square(from) || !valid_square(to) || from == to) return false;

  auto boards = piece_boards();
  PieceBoard *moving = piece_on(boards, bit(from));
  if (!moving || moving->white != white_to_move) return false;

  bool white = moving->white;
  char type = std::toupper((unsigned char)moving->symbol);

  apply_capture(move, to, white, boards);

  *moving->bits = (*moving->bits & ~bit(from)) | bit(to);
  if (move.is_promotion()) {
    *moving->bits &= ~bit(to);
    *promotion_board(move.promotion_piece(), white) |= bit(to);
  }
  move_castle_rook(flag, white);
  update_castling_rights(from, to, type, white);

  en_passant_square = flag == FLAG_DOUBLE_PUSH
                          ? (white ? from + BOARD_SIZE : from - BOARD_SIZE)
                          : -1;
  recompute_aggregates();
  white_to_move = !white_to_move;
  return true;
}
