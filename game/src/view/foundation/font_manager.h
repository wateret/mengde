#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <SDL_ttf.h>
#include <unordered_map>
#include "util/common.h"

class FontManager {
 public:
  FontManager(const string& base_path);
  ~FontManager();
  static FontManager* GetInstance();
  TTF_Font* FetchDefaultFont(int);
  TTF_Font* FetchFont(const string&, int);

 private:
  string base_path_;
  string default_font_name_;
  std::unordered_map<std::string, TTF_Font*> container_;
};

#endif