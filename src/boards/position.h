#pragma once
#include <array>
#include <bit>
#include <cstdint>

struct PieceBoard {
  uint64_t *bits;
  char symbol;
  bool white;
};

std::array<PieceBoard, 12> piece_boards();

struct CastlingRights {
  bool white_short, white_long, black_short, black_long;
};

// game state carried between moves
extern bool white_to_move;
extern CastlingRights rights;
extern int en_passant_square; // square a pawn may capture into, -1 if none

void recompute_aggregates();
bool valid_square(int square);

// visit each set bit's square index, low to high
template <class F> void for_each_square(uint64_t bits, F fn) {
  while (bits) {
    int square = std::countr_zero(bits);
    bits &= bits - 1;
    fn(square);
  }
}

// the piece occupying `mask`, or nullptr if the square is empty
PieceBoard *piece_on(std::array<PieceBoard, 12> &boards, uint64_t mask);

// a full snapshot of the game state, taken before a trial move and rolled back
// after, so legal_moves can test moves without committing them
struct SavedState {
  std::array<uint64_t, 12> boards;
  int en_passant_square;
  CastlingRights rights;
  bool white_to_move;
};

SavedState save_state();
void restore_state(const SavedState &saved);
