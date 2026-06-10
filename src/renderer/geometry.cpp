#include "geometry.h"
#include "board.h"

// screen y is top-down; square 0 (a1) sits bottom-left
int square_at(int px, int py) {
  if (px < 0 || px >= BOARD || py < 0 || py >= BOARD) return -1;
  int file = px / TILE;
  int rank = 7 - (py / TILE);
  return rank * 8 + file;
}

Rectangle square_rect(int square) {
  int file = square % 8;
  int rank = square / 8;
  return {(float)(file * TILE), (float)((7 - rank) * TILE), TILE, TILE};
}

bool hit(Rectangle r, Vector2 p) {
  return p.x >= r.x && p.x < r.x + r.width && p.y >= r.y && p.y < r.y + r.height;
}
