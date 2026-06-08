#include <cstdint>
#include <vector>

struct Moves {
  uint64_t quiet;
  uint64_t captures;
};

struct PawnMoves {
  uint64_t single_advance;
  uint64_t double_advance;
  uint64_t left_capture;
  uint64_t right_capture;
  uint64_t left_en_passant;
  uint64_t right_en_passant;
};

std::vector<uint64_t> pawn_move_generator(uint64_t board, uint64_t pawn_board, bool white_to_move);
std::vector<uint64_t> pawn_capture_generator(uint64_t enemy_board, uint64_t pawn_board, bool white_to_move);
std::vector<uint64_t> en_passant_generator(uint64_t last_enemy_pawns, uint64_t current_enemy_pawns, uint64_t pawn_board, bool white_to_move);

PawnMoves pawn_moves(uint64_t occupied, uint64_t enemy_board,
                     uint64_t last_enemy_pawns, uint64_t current_enemy_pawns,
                     uint64_t pawn_board, bool white_to_move);

Moves bishop_move_generator(uint64_t bishops, uint64_t occupied, uint64_t team_board);
Moves rook_move_generator(uint64_t rooks, uint64_t occupied, uint64_t team_board);
Moves queen_move_generator(uint64_t occupied, uint64_t team_board, bool white_to_move);
Moves knight_move_generator(uint64_t knights, uint64_t occupied, uint64_t team_board);
Moves king_move_generator(uint64_t king, uint64_t occupied, uint64_t team_board);
uint64_t castling_moves(uint64_t occupied, bool white_to_move, bool can_short, bool can_long);
