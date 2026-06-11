#include "textures.h"
#include <string>

#ifndef ASSETS_PATH
#define ASSETS_PATH "assets"
#endif

using std::map;
using std::string;

Textures::Textures() {
  static const map<char, string> files = {
      {'P', "w-pawn"}, {'N', "w-knight"}, {'B', "w-bishop"}, {'R', "w-rook"},
      {'Q', "w-queen"}, {'K', "w-king"}, {'p', "b-pawn"}, {'n', "b-knight"},
      {'b', "b-bishop"}, {'r', "b-rook"}, {'q', "b-queen"}, {'k', "b-king"}};
  for (const auto &[key, value] : files) {
    string path = string(ASSETS_PATH) + "/" + value + ".png";
    Texture2D tex = LoadTexture(path.c_str());
    SetTextureFilter(tex, TEXTURE_FILTER_BILINEAR);
    m_textures[key] = tex;
  }
}

Textures::~Textures() {
  for (auto &[symbol, tex] : m_textures) UnloadTexture(tex);
}
