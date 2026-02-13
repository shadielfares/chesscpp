#include <cstdint>
#include <vector>
std::vector<uint64_t> pawn_move_generator(uint64_t board, uint64_t pawn_board, bool white_to_move);
std::vector<uint64_t> pawn_capture_generator(uint64_t enemy_board, uint64_t pawn_board, bool white_to_move);
std::vector<uint64_t> en_passant_generator(uint64_t last_enemy_pawns, uint64_t current_enemy_pawns, uint64_t pawn_board, bool white_to_move);
