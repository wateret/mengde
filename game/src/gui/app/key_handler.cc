#include "key_handler.h"

namespace mengde {
namespace gui {
namespace app {

void KeyHandler::Register(KeyCmdCode code, std::unique_ptr<KeyCallback>&& callback) {
  map_[code] = std::move(callback);
}

void KeyHandler::Unregister(KeyCmdCode code) {
  auto itr = map_.find(code);
  if (itr == map_.end()) return;
  map_.erase(itr);
}

void KeyHandler::Run(KeyCmdCode code) {
  auto itr = map_.find(code);
  if (itr == map_.end()) return;
  (*map_[code])();
}

}  // namespace app
}  // namespace gui
}  // namespace mengde
