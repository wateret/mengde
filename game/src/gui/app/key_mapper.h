#ifndef MENGDE_GUI_APP_KEY_MAPPER_H_
#define MENGDE_GUI_APP_KEY_MAPPER_H_

#include <unordered_map>

#include <SDL.h>

namespace mengde {
namespace gui {
namespace app {

enum class KeyCmd {
  kNone,
  kBack,
  kCameraLeft,
  kCameraRight,
  kCameraUp,
  kCameraDown,
};

class KeyMapper {
 public:
  KeyMapper() = default;
  void Set(SDL_Keycode keycode, KeyCmd cmd);
  KeyCmd Get(SDL_Keycode keycode);

 private:
  std::unordered_map<SDL_Keycode, KeyCmd> keys_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_KEY_MAPPER_H_
