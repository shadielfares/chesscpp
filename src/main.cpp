#include "boards/board.h"
#include "boards/masks.h"
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

using namespace std;

#define getBit(bitboard, square) (bitboard & (1ULL << square))
#define RANK_4 0xff000000;
#define RANK_5 0xff00000000;

class Chess {
public:
  void Init() { cout << "Bit board init " << endl; }

  // Bit 0 = A1 AKA Layout 2 on Bitboard Calculator
  void printBoard(uint64_t bitboard) {
    for (int rank = 7; rank >= 0; rank--) {
      for (int file = 0; file < 8; file++) {
        int square = rank * 8 + file;
        cout << (getBit(bitboard, square) ? 1 : 0) << " ";
      }
      cout << endl;
    }
  }

  map<string, uint64_t> boardInterface = {};
  void applyMask() {}
};

struct square {
  unsigned int value : 6;
};
struct Flag {
  unsigned int value : 4;
};

/*
 * @param from is 6-bit square
 * @param to is 6-bit square
 * @param flags is a 4-bit encoding that could be EN, Capture, Promotions, or
 * Castle
 */
//unsigned move(uint64_t from, uint64_t to, Flag f) { return mas };

vector<uint64_t> pawnMoveGenerator(uint64_t board, uint64_t pawn_board){
    // Rules for a single pawn move forward
    // 1. There cannot be an allied piece on the target square.
    // 2. There cannot be an enemy piece on the target square.
    uint64_t single_advance_moves = (pawn_board << 8) & ~board; //we need the shifted board ANDED with the empty version of the current board
    // Rules for a double pawn move forward
    // 1. There cannot be an allied piece on the target square AND there cannot be an allied or enemy piece on the first square preceiding the second one.
    // 2. There cannot be an enemy piece on the target square.
    uint64_t double_advance_moves = ((single_advance_moves << 8) & ~board);
    return {single_advance_moves, double_advance_moves };

};

int main() {
  Chess *game = new Chess();
  game->Init();
  uint64_t w_pieces = w_pawn_board | w_king_board | w_queen_board |
                      w_rook_board | w_knight_board | w_bishop_board;
  uint64_t b_pieces = b_pawn_board | b_king_board | b_queen_board |
                      b_rook_board | b_knight_board | b_bishop_board;
  uint64_t occupied_board = w_pieces | b_pieces;
  bool white_to_move = 1; // black_to_move = ~white_to_move = 0;
  uint64_t empty_board = ~occupied_board;
  // game->printBoard(w_pieces | b_pieces);
  vector<uint64_t> moves = pawnMoveGenerator(occupied_board, w_pawn_board);
  game->printBoard(moves.at(0));
  cout << endl;
  game->printBoard(moves.at(1)); //valid double pawn moves
  delete game;
  return 0;
}


struct MakeMoveResponse {
  string from;
  string to;
};

class Player {
public:
  string from, to;
  MakeMoveResponse makeMove() {
    cout << "Enter the current position and target position: ";
    cin >> from >> to;
    return {from, to};
  }
};
