#include <cstdint>
#include <cstdio>
#include <iostream>
#include "boards/board.h"
using namespace std;

class CGameState {
public:
  void Init();
  void Move();
  void Switch();
  void Check();
  void End();
};

class CGameEngine {
public:
  void Init();
  void Move();
  void Switch();
  void Check();
  void End();

private:
  vector<CGameState *> states;
  bool m_running;
};

#define getBit(bitboard, square) (bitboard & (1ULL << square))

class Chess : CGameState {
public:
  void Init() { cout << "Bit board init " << endl; }

  void printBoard(uint64_t bitboard) {
    for (int rank = 0; rank < 8; rank++) {
      for (int file = 0; file < 8; file++) {
        int square = rank * 8 + file;
        cout << (getBit(bitboard, square) ? 1 : 0) << " ";
      }
      cout << endl;
    }
  }
  void move(){}
  void applyMask(){}
};

int main() {
  Chess *game = new Chess();
  game->Init();
  uint64_t board = 0xAA55AA55AA55AA55;
  game->printBoard(w_pawn_board | w_king_board | w_queen_board | w_rook_board | w_knight_board | w_bishop_board);

  delete game;
  return 0;
}
