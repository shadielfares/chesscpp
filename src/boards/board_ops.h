#include "move.h"
#include <string>
std::string board_state();
bool make_move(CMove move);
bool in_check(bool white);
MoveList legal_moves();
