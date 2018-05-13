#ifndef MENGDE_GUI_FOUNDATION_MISC_H_
#define MENGDE_GUI_FOUNDATION_MISC_H_

#include <string>

namespace mengde {
namespace gui {
namespace foundation {

// Misc functions relavant to SDL2

class Misc {
 public:
  static void Init();
  static void Quit();
  static void Delay(int);
  static void SetShowCursor(bool);
  static std::string GetErrorMessage();
};

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_FOUNDATION_MISC_H_
