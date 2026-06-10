#pragma once
#include "controller.h"
#include "model.h"
#include "textures.h"
#include "raylib.h"

// draws one frame: board, hints, pieces, dragged piece and any promotion picker
void draw_scene(const Textures &pieces, const Game &game, const Interaction &ui,
                Vector2 mouse);
