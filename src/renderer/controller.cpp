#include "controller.h"
#include "geometry.h"

void handle_input(Game &game, Interaction &ui, Vector2 mouse) {
  if (ui.promo.active()) {
    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;
    for (int i = 0; i < 4; i++) {
      if (!hit(ui.promo.rect(i), mouse)) continue;
      if (const CMove *m = ui.promo.move_for(PROMO_ORDER[i])) game.play(*m);
      ui.promo = {};
      return;
    }
    return;
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    int sq = square_at((int)mouse.x, (int)mouse.y);
    if (sq >= 0 && game.state[sq] != '.') ui.picked = sq;
  }
  if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && ui.picked >= 0) {
    int target = square_at((int)mouse.x, (int)mouse.y);
    MoveList candidates;
    for (int i = 0; i < game.legal.count; i++) {
      CMove move = game.legal.moves[i];
      if ((int)move.get_from() == ui.picked && (int)move.get_to() == target)
        candidates.add(move);
    }

    if (candidates.count == 1) game.play(candidates.moves[0]);
    else if (candidates.count > 1) ui.promo.moves = candidates;
    ui.picked = -1;
  }
}
