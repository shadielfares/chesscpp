#include "boards/board_ops.h"
#include "raylib.h"
#include <cstdint>
#include <map>
#include <string>

#ifndef ASSETS_PATH
#define ASSETS_PATH "assets"
#endif

using std::map;
using std::string;

static constexpr int TILE = 80;
static constexpr int BOARD = TILE * 8;
static constexpr float SPRITE = 480.0f;

static const Color LIGHT = {240, 217, 181, 255};
static const Color DARK = {181, 136, 99, 255};
static const Color PICK = {130, 151, 105, 200};
static const Color HINT = {20, 20, 20, 90};

// screen y is top-down; square 0 (a1) sits bottom-left
static int square_at(int px, int py) {
  if (px < 0 || px >= BOARD || py < 0 || py >= BOARD) return -1;
  int file = px / TILE;
  int rank = 7 - (py / TILE);
  return rank * 8 + file;
}

static Vector2 square_origin(int square) {
  int file = square % 8;
  int rank = square / 8;
  return {(float)(file * TILE), (float)((7 - rank) * TILE)};
}

static map<char, Texture2D> load_pieces() {
  static const map<char, string> files = {
      {'P', "w-pawn"}, {'N', "w-knight"}, {'B', "w-bishop"}, {'R', "w-rook"},
      {'Q', "w-queen"}, {'K', "w-king"}, {'p', "b-pawn"}, {'n', "b-knight"},
      {'b', "b-bishop"}, {'r', "b-rook"}, {'q', "b-queen"}, {'k', "b-king"}};
  map<char, Texture2D> textures;
  for (const auto &[symbol, name] : files) {
    string path = string(ASSETS_PATH) + "/" + name + ".png";
    Texture2D tex = LoadTexture(path.c_str());
    SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
    textures[symbol] = tex;
  }
  return textures;
}

static void draw_piece(const Texture2D &tex, Vector2 pos) {
  DrawTextureEx(tex, pos, 0.0f, TILE / SPRITE, WHITE);
}

int main() {
  InitWindow(BOARD, BOARD, "chess++");
  SetTargetFPS(60);

  map<char, Texture2D> pieces = load_pieces();
  string state = board_state();
  int picked = -1;
  uint64_t legal = 0;
  bool promoting = false;
  int promo_from = -1, promo_to = -1;
  bool promo_white = true;
  const char promo_options[4] = {'Q', 'R', 'B', 'N'};

  bool over = false;

  while (!WindowShouldClose() && !over) {
    Vector2 mouse = GetMousePosition();

    if (promoting) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int file = promo_to % 8;
        for (int i = 0; i < 4; i++) {
          int row = promo_white ? i : 7 - i;
          float x = file * TILE, y = row * TILE;
          if (mouse.x >= x && mouse.x < x + TILE && mouse.y >= y && mouse.y < y + TILE) {
            if (make_move(promo_from, promo_to, promo_options[i])) {
              state = board_state();
              over = !has_legal_move();
            }
            promoting = false;
            break;
          }
        }
      }
    } else {
      if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        int sq = square_at((int)mouse.x, (int)mouse.y);
        if (sq >= 0 && state[sq] != '.') {
          picked = sq;
          legal = legal_moves(sq);
        }
      }
      if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && picked >= 0) {
        int target = square_at((int)mouse.x, (int)mouse.y);
        if (target >= 0 && (legal & (1ULL << target))) {
          bool is_pawn = state[picked] == 'P' || state[picked] == 'p';
          int rank = target / 8;
          if (is_pawn && (rank == 0 || rank == 7)) {
            promoting = true;
            promo_from = picked;
            promo_to = target;
            promo_white = state[picked] == 'P';
          } else if (make_move(picked, target)) {
            state = board_state();
            over = !has_legal_move();
          }
        }
        picked = -1;
        legal = 0;
      }
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    for (int square = 0; square < 64; square++) {
      Vector2 o = square_origin(square);
      bool light = ((square / 8) + (square % 8)) % 2 != 0;
      DrawRectangle((int)o.x, (int)o.y, TILE, TILE, light ? LIGHT : DARK);
    }
    if (picked >= 0) {
      Vector2 o = square_origin(picked);
      DrawRectangle((int)o.x, (int)o.y, TILE, TILE, PICK);
    }
    for (int square = 0; square < 64; square++) {
      if (!(legal & (1ULL << square))) continue;
      Vector2 o = square_origin(square);
      DrawCircle((int)o.x + TILE / 2, (int)o.y + TILE / 2, TILE * 0.16f, HINT);
    }

    for (int square = 0; square < 64; square++) {
      char symbol = state[square];
      if (symbol == '.' || square == picked) continue;
      draw_piece(pieces[symbol], square_origin(square));
    }
    if (picked >= 0) {
      draw_piece(pieces[state[picked]],
                 {mouse.x - TILE / 2.0f, mouse.y - TILE / 2.0f});
    }
    if (promoting) {
      int file = promo_to % 8;
      for (int i = 0; i < 4; i++) {
        int row = promo_white ? i : 7 - i;
        Vector2 o = {(float)(file * TILE), (float)(row * TILE)};
        DrawRectangle((int)o.x, (int)o.y, TILE, TILE, RAYWHITE);
        DrawRectangleLines((int)o.x, (int)o.y, TILE, TILE, DARK);
        char sym = promo_white ? promo_options[i] : (char)(promo_options[i] + 0x20);
        draw_piece(pieces[sym], o);
      }
    }

    EndDrawing();
  }

  for (auto &[symbol, tex] : pieces) UnloadTexture(tex);
  CloseWindow();
  return 0;
}
