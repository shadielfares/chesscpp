#pragma once
#include "raylib.h"

// choices an underpromotion offers, top-to-bottom for white
inline constexpr char PROMO_ORDER[4] = {'Q', 'R', 'B', 'N'};

// screen pixel -> board square (0 = a1, bottom-left), or -1 if off the board
int square_at(int px, int py);

// board square -> its tile rectangle in screen space
Rectangle square_rect(int square);

// is point p inside rectangle r
bool hit(Rectangle r, Vector2 p);
