#include <cstdint>
#include <iostream>

using namespace std;

#define get_bit(bitboard, square) (bitboard & (1ULL << square))

// Bit 0 = A1 AKA Layout 2 on Bitboard Calculator
void print_board(uint64_t bitboard) {
  for (int rank = 7; rank >= 0; rank--) {
    for (int file = 0; file < 8; file++) {
      int square = rank * 8 + file;
      cout << (get_bit(bitboard, square) ? 1 : 0) << " ";
    }
    cout << endl;
  }
  cout << '\n';
};
