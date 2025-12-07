#include <cstdint>
#include <cstdio>
#include <iostream>

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

class Chess : CGameState {
public:
  void Init() {
    uint64_t board = 0x00000000000000000000;
    cout << "Bit board init " << board << endl;
  }

  void printBoard(uint64_t board) {
    for (int rank = 0; rank < 8; rank++) {
      for (int file = 0; file < 8; file++) {
        int square = rank * 8 + file;
        cout << (square & (1ULL << square) ? 1 : 0) << " ";
      }
      cout << endl;
    }
  }
};

int main() {
  Chess *game = new Chess();
  game->Init();
  uint64_t board = 1ULL;
  game->printBoard(board);

  delete game;
  return 0;
}
