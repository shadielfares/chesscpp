#pragma once
#include <cstdint>

// back-rank bit indices: every square castling touches (king/rook homes, the
// squares they pass over). LSB layout, A1 = 0 ... H8 = 63.
enum Square : int {
  A1 = 0, B1 = 1, C1 = 2, D1 = 3, E1 = 4, F1 = 5, G1 = 6, H1 = 7,
  A8 = 56, B8 = 57, C8 = 58, D8 = 59, E8 = 60, F8 = 61, G8 = 62, H8 = 63,
};

constexpr int BOARD_SIZE = 8;
constexpr int BOARD_SQUARES = BOARD_SIZE * BOARD_SIZE;

constexpr int file_of(int square) { return square % BOARD_SIZE; }
constexpr int rank_of(int square) { return square / BOARD_SIZE; }

constexpr uint64_t bit(int square) { return 1ULL << square; }
