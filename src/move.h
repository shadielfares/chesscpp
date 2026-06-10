#pragma once
#include <cstdint>

// A move's flag occupies bits 12-15 of the packed move. Within the flag,
// bit 2 marks a capture and bit 3 marks a promotion.
enum MoveFlag {
  FLAG_QUIET = 0,
  FLAG_DOUBLE_PUSH = 1,
  FLAG_CASTLE_SHORT = 2,
  FLAG_CASTLE_LONG = 3,
  FLAG_CAPTURE = 4,
  FLAG_EP_CAPTURE = 5,
  FLAG_PROMO_N = 8,
  FLAG_PROMO_B = 9,
  FLAG_PROMO_R = 10,
  FLAG_PROMO_Q = 11,
  FLAG_PROMO_N_CAPTURE = 12,
  FLAG_PROMO_B_CAPTURE = 13,
  FLAG_PROMO_R_CAPTURE = 14,
  FLAG_PROMO_Q_CAPTURE = 15,
};

// A move packed into 16 bits: to (bits 0-5), from (bits 6-11), flag (12-15).
// The flag carries the move's semantics so make_move never has to re-derive
// them.
class CMove {
public:
  CMove() = default; // uninitialized; only filled slots of a MoveList are read
  CMove(unsigned int from, unsigned int to, MoveFlag flag) {
    m_move = (uint16_t)(((flag & FLAG_MASK) << FLAG_SHIFT) |
                        ((from & SQUARE_MASK) << FROM_SHIFT) |
                        (to & SQUARE_MASK));
  }

  unsigned int get_to() const { return m_move & SQUARE_MASK; }
  unsigned int get_from() const { return (m_move >> FROM_SHIFT) & SQUARE_MASK; }
  unsigned int get_flag() const { return (m_move >> FLAG_SHIFT) & FLAG_MASK; }

  bool is_capture() const { return get_flag() & CAPTURE_BIT; }
  bool is_promotion() const { return get_flag() & PROMOTION_BIT; }

  // piece letter a promotion flag resolves to (low two bits pick N/B/R/Q)
  char promotion_piece() const {
    switch (get_flag() & PROMO_PIECE_MASK) {
    case 0: return 'N';
    case 1: return 'B';
    case 2: return 'R';
    default: return 'Q';
    }
  }

private:
  static constexpr int FROM_SHIFT = 6;
  static constexpr int FLAG_SHIFT = 12;
  static constexpr unsigned int SQUARE_MASK = 0x3f; // six bits, one square
  static constexpr unsigned int FLAG_MASK = 0xf;    // four bits, one flag
  static constexpr unsigned int CAPTURE_BIT = 0x4;
  static constexpr unsigned int PROMOTION_BIT = 0x8;
  static constexpr unsigned int PROMO_PIECE_MASK = 0x3;

  uint16_t m_move;
};

// The most legal moves found in any legal position is 218 (a constructed
// many-queens position), so a fixed stack buffer holds any move list without
// heap allocation. 256 leaves headroom.
struct MoveList {
  static constexpr int CAPACITY = 256;
  CMove moves[CAPACITY];
  int count = 0;

  void add(CMove move) { moves[count++] = move; }
  bool empty() const { return count == 0; }
};
