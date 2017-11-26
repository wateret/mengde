#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <string>
#include <unordered_map>
#include "texture.h"

using std::string;
using std::unordered_map;

class TextureManager {
 public:
  TextureManager(Renderer* renderer);
  ~TextureManager();

 public:
  Texture* FetchTexture(const string&);
  Texture* FetchWhitenedTexture(const string&);
  Texture* FetchTextTexture(const string&, int, Color, uint32_t = 0);

 private:
  unordered_map<string, Texture*> container_;
  Texture*  dummy_texture_;
  Renderer* renderer_;
};

#endif // TEXTURE_MANAGER_H_
