#ifndef TEXTURE_MANAGER_H_
#define TEXTURE_MANAGER_H_

#include <string>
#include <unordered_map>
#include "texture.h"

namespace mengde {
namespace gui {
namespace foundation {

using std::string;

class FontManager;

// TextureManager manages all kind of textures like sprites and rendered texts
// It acts like a cache for textures. A texture can be found with string key.
// FIXME Caching text textures may cause memory explosion

class TextureManager {
 public:
  TextureManager(Renderer* renderer, const string& bitmap_base_path, const string& font_base_path);
  ~TextureManager();

 public:
  Texture* FetchTexture(const string&);
  Texture* FetchWhitenedTexture(const string&);
  Texture* FetchTextTexture(const string&, int, Color, uint32_t = 0);
  void     SetBitmapBasePath(const string& path) { bitmap_base_path_ = path; }

 private:
  string                               bitmap_base_path_;
  FontManager*                         font_manager_;
  std::unordered_map<string, Texture*> container_;
  Texture*                             dummy_texture_;
  Renderer*                            renderer_;
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // TEXTURE_MANAGER_H_
