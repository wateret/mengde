#ifndef VIEW_FOUNDATION_MISC_H_
#define VIEW_FOUNDATION_MISC_H_

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

#endif // VIEW_FOUNDATION_MISC_H_
