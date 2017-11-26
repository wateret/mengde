#ifndef MISC_H
#define MISC_H

#include <string>

// Misc functions relavant to SDL2

class Misc {
 public:
  static void Init();
  static void Quit();
  static void Delay(int);
  static void SetShowCursor(bool);
  static std::string GetErrorMessage();
};

#endif
