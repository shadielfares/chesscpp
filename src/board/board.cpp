#include <iostream>
#include <stdint.h>

using namespace std;

class BoardI {
};

class Board : public BoardI {
private:
  unsigned int board = 0;
};
