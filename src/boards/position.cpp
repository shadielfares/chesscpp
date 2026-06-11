#include "boards/position.h"
#include "boards/bitboards.h"
#include "boards/board_ops.h"
#include "boards/squares.h"

using std::array;
using std::string;

array<PieceBoard, 12> piece_boards() {
  return {{
      {&w_pawn_board, 'P', true},
      {&w_knight_board, 'N', true},
      {&w_bishop_board, 'B', true},
      {&w_rook_board, 'R', true},
      {&w_queen_board, 'Q', true},
      {&w_king_board, 'K', true},
      {&b_pawn_board, 'p', false},
      {&b_knight_board, 'n', false},
      {&b_bishop_board, 'b', false},
      {&b_rook_board, 'r', false},
      {&b_queen_board, 'q', false},
      {&b_king_board, 'k', false},
  }};
}

bool white_to_move = true;
CastlingRights rights = {true, true, true, true};
int en_passant_square = -1;

void recompute_aggregates() {
  w_pieces = w_pawn_board | w_knight_board | w_bishop_board | w_rook_board |
             w_queen_board | w_king_board;
  b_pieces = b_pawn_board | b_knight_board | b_bishop_board | b_rook_board |
             b_queen_board | b_king_board;
}

bool valid_square(int square) { return square >= 0 && square < BOARD_SQUARES; }

PieceBoard *piece_on(array<PieceBoard, 12> &boards, uint64_t mask) {
  for (PieceBoard &entry : boards)
    if (*entry.bits & mask) return &entry;
  return nullptr;
}

// index i maps to square i; '.' for empty, piece letter otherwise
string board_state() {
  string state(BOARD_SQUARES, '.');
  for (const PieceBoard &entry : piece_boards())
    for_each_square(*entry.bits, [&](int sq) { state[sq] = entry.symbol; });
  return state;
}

SavedState save_state() {
  SavedState saved{};
  int i = 0;
  for (const PieceBoard &entry : piece_boards()) saved.boards[i++] = *entry.bits;
  saved.en_passant_square = en_passant_square;
  saved.rights = rights;
  saved.white_to_move = white_to_move;
  return saved;
}

void restore_state(const SavedState &saved) {
  int i = 0;
  for (const PieceBoard &entry : piece_boards()) *entry.bits = saved.boards[i++];
  en_passant_square = saved.en_passant_square;
  rights = saved.rights;
  white_to_move = saved.white_to_move;
  recompute_aggregates();
}
