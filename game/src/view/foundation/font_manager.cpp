#include "font_manager.h"
#include "util/common.h"
#include "util/game_env.h"

#include <iostream>

FontManager* FontManager::GetInstance() {
  static FontManager instance;
  return &instance;
}

FontManager::FontManager() {
  TTF_Init();

  default_font_name_ = "Literation Mono Powerline";
//  default_font_name_ = "NanumGothicCodingBold"; // Default Font for Hangul
}

FontManager::~FontManager() {
  for (const auto& u : container_) {
    TTF_Font* font = u.second;
    TTF_CloseFont(font);
  }

  TTF_Quit();
}

TTF_Font* FontManager::FetchFont(const string& name, int size) {
  UNUSED(name);

  // FIXME Do not use Path class(remove base path and get full path from caller instead)
  Path base_path = GameEnv::GetInstance()->GetResourcePath();
  Path full_path = base_path / (default_font_name_ + ".ttf");
  std::string key = default_font_name_ + ":" + std::to_string(size);

  auto iter = container_.find(key);
  if (iter == container_.end()) {
    TTF_Font* font = TTF_OpenFont(full_path.ToString().c_str(), size);
    container_[key] = font;
    return font;
  } else {
    return iter->second;
  }
}

TTF_Font* FontManager::FetchDefaultFont(int size) {
  return FetchFont(default_font_name_, size);
}

