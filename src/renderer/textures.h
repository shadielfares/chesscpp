#pragma once
#include "raylib.h"
#include <map>

// owns every piece sprite as a GPU texture and unloads them all on destruction,
class Textures {
public:
  Textures();
  ~Textures();
  Textures(const Textures &) = delete;
  Textures &operator=(const Textures &) = delete;

  // texture for a board symbol ('P', 'k', ...)
  const Texture2D &at(char symbol) const { return m_textures.at(symbol); }

private:
  std::map<char, Texture2D> m_textures;
};
