#pragma once
#include "raylib.h"
#include "move.h"
#include <string>

// the model: board state plus the legal replies to it, kept in sync after every
// move so the rest of the gui only ever reads a consistent snapshot
struct Game {
  std::string state;
  MoveList legal;
  bool over;

  Game();
  void play(CMove move);
};

// a pending promotion: a set of moves sharing one from/to, differing only by the
// promoted piece, laid out as a vertical picker over the destination file
struct Promo {
  MoveList moves;

  bool active() const { return !moves.empty(); }
  bool white() const { return moves.moves[0].get_to() / 8 == 7; }
  int file() const { return moves.moves[0].get_to() % 8; }

  // square the i-th choice in PROMO_ORDER occupies; white picks downward from
  // rank 8, black upward from rank 1
  Rectangle rect(int i) const;
  const CMove *move_for(char piece) const;
};
