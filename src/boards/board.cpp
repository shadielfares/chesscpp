#include <cstdint>
/*
 * This module uses Layout #2 in the following site: https://gekomad.github.io/Cinnamon/BitboardCalculator/
 * We use this layout due to the natural order of Little Endian Mapping or LSB mapping
 * This is wherw a < h and 0 < 7
 */

uint64_t w_pawn_board = 0xff00;
uint64_t b_pawn_board = 0xff000000000000;

uint64_t w_rook_board = 0x81;
uint64_t b_rook_board = 0x8100000000000000;

uint64_t w_knight_board = 0x42;
uint64_t b_knight_board = 0x4200000000000000;

uint64_t w_bishop_board = 0x24;
uint64_t b_bishop_board = 0x2400000000000000;

uint64_t w_queen_board = 0x8;
uint64_t b_queen_board = 0x800000000000000;

uint64_t w_king_board = 0x10;
uint64_t b_king_board = 0x1000000000000000;

uint64_t w_pieces = w_pawn_board | w_king_board | w_queen_board |
                    w_rook_board | w_knight_board | w_bishop_board;
uint64_t b_pieces = b_pawn_board | b_king_board | b_queen_board |
                    b_rook_board | b_knight_board | b_bishop_board;
