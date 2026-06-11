#include "gui.h"
#include "controller.h"
#include "view.h"
#include "model.h"
#include "textures.h"
#include "board.h"
#include "raylib.h"

// owns the raylib window; opening and closing are a resource pair, so tie the
// close to the lifetime instead of a manual call
struct Window {
  Window(int w, int h, const char *title) { InitWindow(w, h, title); }
  ~Window() { CloseWindow(); }
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;
};

void gui_handler() {
  Window window(BOARD, BOARD, "chess++");
  SetTargetFPS(60);

  Textures pieces;
  Game game;
  Interaction ui;

  while (!WindowShouldClose() && !game.over) {
    Vector2 mouse = GetMousePosition();
    handle_input(game, ui, mouse);

    BeginDrawing();
    draw_scene(pieces, game, ui, mouse);
    EndDrawing();
  }
}
