#include "boards/board.h"
#include "boards/board_ops.h"
#include "boards/squares.h"
#include "pieces/index.h"
#include <array>
#include <bit>
#include <cstdint>
#include <string>

using std::array;
using std::string;

namespace {

struct PieceBoard {
  uint64_t *bits;
  char symbol;
  bool white;
};

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

struct CastlingRights {
  bool white_short, white_long, black_short, black_long;
};

// game state carried between moves
bool white_to_move = true;
CastlingRights rights = {true, true, true, true};
int en_passant_square = -1; // square a pawn may capture into, -1 if none

void recompute_aggregates() {
  w_pieces = w_pawn_board | w_knight_board | w_bishop_board | w_rook_board |
             w_queen_board | w_king_board;
  b_pieces = b_pawn_board | b_knight_board | b_bishop_board | b_rook_board |
             b_queen_board | b_king_board;
}

bool valid_square(int square) { return square >= 0 && square < 64; }

} // namespace

// index i maps to square i; '.' for empty, piece letter otherwise
string board_state() {
  string state(64, '.');
  for (const PieceBoard &entry : piece_boards()) {
    uint64_t bits = *entry.bits;
    while (bits) {
      int square = std::countr_zero(bits);
      state[square] = entry.symbol;
      bits &= bits - 1;
    }
  }
  return state;
}

bool is_white_to_move() { return white_to_move; }

// applies a move for the side to move, handling captures, en passant,
// castling, promotion, and rights/turn updates. `promotion` picks the promoted
// piece (Q/R/B/N) when a pawn reaches the far rank.
bool make_move(int from, int to, char promotion) {
  if (!valid_square(from) || !valid_square(to) || from == to) return false;

  uint64_t from_mask = bit(from);
  uint64_t to_mask = bit(to);

  auto boards = piece_boards();
  PieceBoard *moving = nullptr;
  for (PieceBoard &entry : boards)
    if (*entry.bits & from_mask) {
      moving = &entry;
      break;
    }
  if (!moving || moving->white != white_to_move) return false;

  bool white = moving->white;
  char type = moving->symbol & ~0x20;
  int next_en_passant = -1;

  // en passant: pawn lands on the ep square, remove the pawn it passed
  if (type == 'P' && to == en_passant_square) {
    uint64_t captured = bit(white ? to - 8 : to + 8);
    (white ? b_pawn_board : w_pawn_board) &= ~captured;
  }

  // normal capture: clear any enemy piece sitting on the target
  for (PieceBoard &entry : boards)
    if (entry.white != white) *entry.bits &= ~to_mask;

  *moving->bits &= ~from_mask;
  *moving->bits |= to_mask;

  // promotion: pawn reaching the far rank becomes the chosen piece
  if (type == 'P') {
    int rank = to / 8;
    if ((white && rank == 7) || (!white && rank == 0)) {
      *moving->bits &= ~to_mask;
      uint64_t *promoted;
      switch (promotion & ~0x20) {
      case 'N': promoted = white ? &w_knight_board : &b_knight_board; break;
      case 'B': promoted = white ? &w_bishop_board : &b_bishop_board; break;
      case 'R': promoted = white ? &w_rook_board : &b_rook_board; break;
      default: promoted = white ? &w_queen_board : &b_queen_board; break;
      }
      *promoted |= to_mask;
    }
  }

  // castling: king jumped two files, hop the rook over it
  if (type == 'K') {
    if (from == E1 && to == G1) w_rook_board = (w_rook_board & ~bit(H1)) | bit(F1);
    else if (from == E1 && to == C1) w_rook_board = (w_rook_board & ~bit(A1)) | bit(D1);
    else if (from == E8 && to == G8) b_rook_board = (b_rook_board & ~bit(H8)) | bit(F8);
    else if (from == E8 && to == C8) b_rook_board = (b_rook_board & ~bit(A8)) | bit(D8);
  }

  // double pawn push arms en passant for the square it skipped
  if (type == 'P' && (to - from == 16 || from - to == 16))
    next_en_passant = white ? from + 8 : from - 8;

  // castling rights: king/rook leaving home, or a home rook captured
  if (type == 'K') {
    if (white) rights.white_short = rights.white_long = false;
    else rights.black_short = rights.black_long = false;
  }
  if (from == A1 || to == A1) rights.white_long = false;
  if (from == H1 || to == H1) rights.white_short = false;
  if (from == A8 || to == A8) rights.black_long = false;
  if (from == H8 || to == H8) rights.black_short = false;

  en_passant_square = next_en_passant;
  recompute_aggregates();
  white_to_move = !white_to_move;
  return true;
}

namespace {

// all squares the given side attacks. team_board = 0 makes each generator
// report its full attack set (rays stop at the first blocker, inclusive).
uint64_t squares_attacked_by(bool by_white) {
  uint64_t occupied = w_pieces | b_pieces;
  uint64_t pawns = by_white ? w_pawn_board : b_pawn_board;
  uint64_t knights = by_white ? w_knight_board : b_knight_board;
  uint64_t bishops = by_white ? w_bishop_board : b_bishop_board;
  uint64_t rooks = by_white ? w_rook_board : b_rook_board;
  uint64_t king = by_white ? w_king_board : b_king_board;

  uint64_t attacks = 0;
  auto pawn_caps = pawn_capture_generator(~0ULL, pawns, by_white);
  attacks |= pawn_caps.at(0) | pawn_caps.at(1);

  Moves knight_moves = knight_move_generator(knights, occupied, 0);
  Moves bishop_moves = bishop_move_generator(bishops, occupied, 0);
  Moves rook_moves = rook_move_generator(rooks, occupied, 0);
  Moves queen_moves = queen_move_generator(occupied, 0, by_white);
  Moves king_moves = king_move_generator(king, occupied, 0);

  attacks |= knight_moves.quiet | knight_moves.captures;
  attacks |= bishop_moves.quiet | bishop_moves.captures;
  attacks |= rook_moves.quiet | rook_moves.captures;
  attacks |= queen_moves.quiet | queen_moves.captures;
  attacks |= king_moves.quiet | king_moves.captures;
  return attacks;
}

// castle targets whose whole king path (start, transit, destination) is free of
// attack, so the king never starts in, passes through, or lands on check.
uint64_t castle_targets(uint64_t occupied, bool white) {
  bool can_short = white ? rights.white_short : rights.black_short;
  bool can_long = white ? rights.white_long : rights.black_long;
  uint64_t castles = castling_moves(occupied, white, can_short, can_long);
  if (!castles) return 0;

  uint64_t attacked = squares_attacked_by(!white);
  struct CastleOption {
    int target;
    uint64_t king_path;
  };
  const CastleOption options[4] = {
      {G1, bit(E1) | bit(F1) | bit(G1)},
      {C1, bit(E1) | bit(D1) | bit(C1)},
      {G8, bit(E8) | bit(F8) | bit(G8)},
      {C8, bit(E8) | bit(D8) | bit(C8)},
  };

  uint64_t safe = 0;
  for (const CastleOption &option : options)
    if ((castles & bit(option.target)) && !(option.king_path & attacked))
      safe |= bit(option.target);
  return safe;
}

// pseudo-legal targets for the single piece on `square`. king-safety is
// filtered later in legal_moves; castling path-safety is handled here.
uint64_t pseudo_targets(int square, bool white) {
  uint64_t occupied = w_pieces | b_pieces;
  uint64_t team = white ? w_pieces : b_pieces;
  uint64_t enemy = white ? b_pieces : w_pieces;
  uint64_t piece = bit(square);

  char type = 0;
  for (const PieceBoard &entry : piece_boards())
    if (*entry.bits & piece) type = entry.symbol & ~0x20; // uppercase

  switch (type) {
  case 'P': {
    PawnMoves pawn = pawn_moves(occupied, enemy, 0, 0, piece, white);
    uint64_t targets = pawn.single_advance | pawn.double_advance |
                       pawn.left_capture | pawn.right_capture;
    if (en_passant_square >= 0) {
      auto ep_caps = pawn_capture_generator(bit(en_passant_square), piece, white);
      targets |= ep_caps.at(0) | ep_caps.at(1);
    }
    return targets;
  }
  case 'N': {
    Moves m = knight_move_generator(piece, occupied, team);
    return m.quiet | m.captures;
  }
  case 'B': {
    Moves m = bishop_move_generator(piece, occupied, team);
    return m.quiet | m.captures;
  }
  case 'R': {
    Moves m = rook_move_generator(piece, occupied, team);
    return m.quiet | m.captures;
  }
  case 'Q': {
    Moves diagonal = bishop_move_generator(piece, occupied, team);
    Moves straight = rook_move_generator(piece, occupied, team);
    return diagonal.quiet | diagonal.captures | straight.quiet | straight.captures;
  }
  case 'K': {
    Moves m = king_move_generator(piece, occupied, team);
    return m.quiet | m.captures | castle_targets(occupied, white);
  }
  }
  return 0;
}

struct SavedState {
  array<uint64_t, 12> boards;
  int en_passant_square;
  CastlingRights rights;
  bool white_to_move;
};

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

} // namespace

bool in_check(bool white) {
  uint64_t king = white ? w_king_board : b_king_board;
  return king & squares_attacked_by(!white);
}

// legal target squares for the piece on `square`: pseudo-moves filtered to
// those that don't leave the mover's own king in check. only the side to move
// has legal moves.
uint64_t legal_moves(int square) {
  if (!valid_square(square)) return 0;
  uint64_t piece = bit(square);

  bool white = false;
  bool found = false;
  for (const PieceBoard &entry : piece_boards())
    if (*entry.bits & piece) {
      white = entry.white;
      found = true;
    }
  if (!found || white != white_to_move) return 0;

  uint64_t targets = pseudo_targets(square, white);
  uint64_t legal = 0;
  while (targets) {
    int to = std::countr_zero(targets);
    SavedState saved = save_state();
    make_move(square, to);
    if (!in_check(white)) legal |= bit(to);
    restore_state(saved);
    targets &= targets - 1;
  }
  return legal;
}

// true while the side to move has at least one legal move; false = game over
// (checkmate if currently in check, otherwise stalemate).
bool has_legal_move() {
  for (int square = 0; square < 64; square++)
    if (legal_moves(square)) return true;
  return false;
}
