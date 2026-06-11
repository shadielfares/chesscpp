#include "view.h"
#include "geometry.h"
#include "board.h"

static void draw_piece(const Texture2D &tex, Vector2 pos) {
  DrawTextureEx(tex, pos, 0.0f, TILE / SPRITE, WHITE);
}

void draw_scene(const Textures &pieces, const Game &game, const Interaction &ui,
                Vector2 mouse) {
  ClearBackground(RAYWHITE);

  for (int square = 0; square < 64; square++) {
    bool light = ((square / 8) + (square % 8)) % 2 != 0;
    DrawRectangleRec(square_rect(square), light ? LIGHT : DARK);
  }
  if (ui.picked >= 0) {
    DrawRectangleRec(square_rect(ui.picked), PICK);
    for (int i = 0; i < game.legal.count; i++) {
      if ((int)game.legal.moves[i].get_from() != ui.picked) continue;
      Rectangle c = square_rect(game.legal.moves[i].get_to());
      DrawCircle((int)c.x + TILE / 2, (int)c.y + TILE / 2, TILE * 0.16f, HINT);
    }
  }

  for (int square = 0; square < 64; square++) {
    char symbol = game.state[square];
    if (symbol == '.' || square == ui.picked) continue;
    Rectangle r = square_rect(square);
    draw_piece(pieces.at(symbol), {r.x, r.y});
  }
  if (ui.picked >= 0) {
    draw_piece(pieces.at(game.state[ui.picked]),
               {mouse.x - TILE / 2.0f, mouse.y - TILE / 2.0f});
  }
  if (ui.promo.active()) {
    for (int i = 0; i < 4; i++) {
      Rectangle r = ui.promo.rect(i);
      DrawRectangleRec(r, RAYWHITE);
      DrawRectangleLines((int)r.x, (int)r.y, TILE, TILE, DARK);
      char sym = ui.promo.white() ? PROMO_ORDER[i] : (char)(PROMO_ORDER[i] + 0x20);
      draw_piece(pieces.at(sym), {r.x, r.y});
    }
  }
}
