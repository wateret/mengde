#include "key_mapper.h"

namespace mengde {
namespace gui {
namespace app {

void KeyMapper::Set(SDL_Keycode keycode, KeyCmd cmd) { keys_[keycode] = cmd; }

KeyCmd KeyMapper::Get(SDL_Keycode keycode) {
  if (keys_.find(keycode) == keys_.end()) return KeyCmd::kNone;
  return keys_.at(keycode);
}

}  // namespace app
}  // namespace gui
}  // namespace mengde
