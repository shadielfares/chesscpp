#pragma once
#include "model.h"
#include "raylib.h"

// the controller's transient state: which piece is being dragged and whether a
// promotion choice is pending
struct Interaction {
  int picked = -1;
  Promo promo;
};

// advances game/ui from one frame's mouse state: drag-to-move and promotion pick
void handle_input(Game &game, Interaction &ui, Vector2 mouse);
