#include <iostream>
using namespace std;

template <typename T> T moveNumber() {
  T spaces;
  bool validMove = false;
  int move;
  while (validMove == false) {
    cin >> move;
    if (move <= 8 || move >= 1) {
      validMove = true;
    } else {
      cout << "Invalid input, try again..." << endl;
    }
  }
}

class BoardI {
public:
  virtual void move(); // Conducts the moving;
private:
  virtual void convertIntToUInt(int moveNumber);
};

class Board : public BoardI {
private:
  unsigned int board = 0;
};
