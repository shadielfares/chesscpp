#include "boards/attacks.h"
#include "boards/bitboards.h"
#include "boards/board_ops.h"
#include "boards/masks.h"
#include "boards/position.h"
#include "boards/squares.h"
#include "move.h"
#include <cctype>

constexpr int CASTLE_FILE_SPAN = 2; // files the king crosses when castling

// tags a pawn's target bitboard with flags: promotions expand to four moves,
// the ep square becomes an ep capture, a two-rank jump is a double push.
static void add_pawn_moves(MoveList &moves, int from, uint64_t targets, bool white) {
  uint64_t enemy = white ? b_pieces : w_pieces;
  for_each_square(targets, [&](int to) {
    bool promotion = (white && (bit(to) & RANK_8)) || (!white && (bit(to) & RANK_1));
    bool capture = bit(to) & enemy;
    int step = to > from ? to - from : from - to;

    if (promotion && capture) {
      moves.add(CMove(from, to, FLAG_PROMO_Q_CAPTURE));
      moves.add(CMove(from, to, FLAG_PROMO_R_CAPTURE));
      moves.add(CMove(from, to, FLAG_PROMO_B_CAPTURE));
      moves.add(CMove(from, to, FLAG_PROMO_N_CAPTURE));
    } else if (promotion) {
      moves.add(CMove(from, to, FLAG_PROMO_Q));
      moves.add(CMove(from, to, FLAG_PROMO_R));
      moves.add(CMove(from, to, FLAG_PROMO_B));
      moves.add(CMove(from, to, FLAG_PROMO_N));
    } else if (to == en_passant_square) {
      moves.add(CMove(from, to, FLAG_EP_CAPTURE));
    } else if (capture) {
      moves.add(CMove(from, to, FLAG_CAPTURE));
    } else if (step == 2 * BOARD_SIZE) {
      moves.add(CMove(from, to, FLAG_DOUBLE_PUSH));
    } else {
      moves.add(CMove(from, to, FLAG_QUIET));
    }
  });
}

// tags a non-pawn piece's target bitboard. a king landing two files away is a
// castle (g-file short, c-file long).
static void add_piece_moves(MoveList &moves, int from, uint64_t targets,
                            bool white, bool is_king) {
  uint64_t enemy = white ? b_pieces : w_pieces;
  for_each_square(targets, [&](int to) {
    int file_step = file_of(to) - file_of(from);
    if (file_step < 0) file_step = -file_step;

    if (is_king && file_step == CASTLE_FILE_SPAN)
      moves.add(CMove(from, to, (to == G1 || to == G8) ? FLAG_CASTLE_SHORT
                                                        : FLAG_CASTLE_LONG));
    else if (bit(to) & enemy)
      moves.add(CMove(from, to, FLAG_CAPTURE));
    else
      moves.add(CMove(from, to, FLAG_QUIET));
  });
}

// every pseudo-legal move for the side to move, each tagged with its flag.
// king safety is filtered in legal_moves.
static MoveList generate_pseudo(bool white) {
  MoveList moves;
  for (const PieceBoard &entry : piece_boards()) {
    if (entry.white != white) continue;
    char type = std::toupper((unsigned char)entry.symbol);
    for_each_square(*entry.bits, [&](int from) {
      uint64_t targets = pseudo_targets(from, white);
      if (type == 'P') add_pawn_moves(moves, from, targets, white);
      else add_piece_moves(moves, from, targets, white, type == 'K');
    });
  }
  return moves;
}

// legal moves for the side to move: pseudo moves filtered to those that don't
// leave the mover's own king in check. an empty list means game over
// (checkmate if currently in check, otherwise stalemate).
MoveList legal_moves() {
  bool white = white_to_move;
  MoveList pseudo = generate_pseudo(white);
  MoveList legal;
  for (int i = 0; i < pseudo.count; i++) {
    CMove move = pseudo.moves[i];
    SavedState saved = save_state();
    make_move(move);
    if (!in_check(white)) legal.add(move);
    restore_state(saved);
  }
  return legal;
}
