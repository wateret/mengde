#ifndef MENGDE_GUI_FOUNDATION_FONT_MANAGER_H_
#define MENGDE_GUI_FOUNDATION_FONT_MANAGER_H_

#include <SDL_ttf.h>
#include <unordered_map>
#include "util/common.h"

namespace mengde {
namespace gui {
namespace foundation {

class FontManager {
 public:
  FontManager(const string& base_path);
  ~FontManager();
  static FontManager* GetInstance();
  TTF_Font*           FetchDefaultFont(int);
  TTF_Font*           FetchFont(const string&, int);

 private:
  string                                     base_path_;
  string                                     default_font_name_;
  std::unordered_map<std::string, TTF_Font*> container_;
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif // MENGDE_GUI_FOUNDATION_FONT_MANAGER_H_
