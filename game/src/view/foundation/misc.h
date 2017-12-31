#ifndef MISC_H
#define MISC_H

#include <string>
#include "sprite_type.h"

// Misc functions relavant to SDL2

class Misc {
 public:
  static void Init();
  static void Quit();
  static void Delay(int);
  static void SetShowCursor(bool);
  static std::string GetErrorMessage();
  static std::string GetModelPath(const std::string& /* model_id */, SpriteType);
};

#endif
