#include "model.h"
#include "board.h"
#include "boards/board_ops.h"

Game::Game() {
  state = board_state();
  legal = legal_moves();
  over = legal.empty();
}

void Game::play(CMove move) {
  make_move(move);
  state = board_state();
  legal = legal_moves();
  over = legal.empty();
}

Rectangle Promo::rect(int i) const {
  int row = white() ? i : 7 - i;
  return {(float)(file() * TILE), (float)(row * TILE), TILE, TILE};
}

const CMove *Promo::move_for(char piece) const {
  for (int c = 0; c < moves.count; c++)
    if (moves.moves[c].promotion_piece() == piece) return &moves.moves[c];
  return nullptr;
}
