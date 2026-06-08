#include <cstdint>
#include <string>
std::string board_state();
bool make_move(int from, int to, char promotion = 'Q');
bool in_check(bool white);
bool is_white_to_move();
uint64_t legal_moves(int square);
bool has_legal_move();
