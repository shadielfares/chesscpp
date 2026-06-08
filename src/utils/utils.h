#pragma once
#include <cstdint>

// Bit 0 = A1 AKA Layout 2 on Bitboard Calculator
inline bool get_bit(uint64_t bitboard, int square) {
  return bitboard & (1ULL << square);
}

void print_board(uint64_t bitboard);
