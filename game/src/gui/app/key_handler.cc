#include "key_handler.h"

namespace mengde {
namespace gui {
namespace app {

void KeyHandler::Register(Event event, std::unique_ptr<KeyCallback>&& callback) {
  auto itr = map_.find(event);
  if (itr == map_.end()) {
    map_.insert(std::make_pair(event, std::move(callback)));
  } else {
    itr->second = std::move(callback);
  }
}

void KeyHandler::Unregister(Event event) {
  auto itr = map_.find(event);
  if (itr == map_.end()) return;
  map_.erase(itr);
}

void KeyHandler::Run(Event event) {
  auto itr = map_.find(event);
  if (itr == map_.end()) return;
  (*map_[event])();
}

}  // namespace app
}  // namespace gui
}  // namespace mengde
