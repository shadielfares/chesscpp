#include "boards/attacks.h"
#include "boards/bitboards.h"
#include "boards/board_ops.h"
#include "boards/position.h"
#include "boards/squares.h"
#include "pieces/index.h"
#include <cctype>

// all squares the given side attacks. team_board = 0 makes each generator
// report its full attack set (rays stop at the first blocker, inclusive).
static uint64_t squares_attacked_by(bool by_white) {
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

bool in_check(bool white) {
  uint64_t king = white ? w_king_board : b_king_board;
  return king & squares_attacked_by(!white);
}

struct CastleOption {
  int target;
  uint64_t king_path;
};

// castle targets whose whole king path (start, transit, destination) is free of
// attack, so the king never starts in, passes through, or lands on check.
static uint64_t castle_targets(uint64_t occupied, bool white) {
  bool can_short = white ? rights.white_short : rights.black_short;
  bool can_long = white ? rights.white_long : rights.black_long;
  uint64_t castles = castling_moves(occupied, white, can_short, can_long);
  if (!castles) return 0;

  uint64_t attacked = squares_attacked_by(!white);
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

uint64_t pseudo_targets(int square, bool white) {
  uint64_t occupied = w_pieces | b_pieces;
  uint64_t team = white ? w_pieces : b_pieces;
  uint64_t enemy = white ? b_pieces : w_pieces;
  uint64_t piece = bit(square);

  char type = 0;
  for (const PieceBoard &entry : piece_boards())
    if (*entry.bits & piece) type = std::toupper((unsigned char)entry.symbol);

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
