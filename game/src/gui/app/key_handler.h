#ifndef MENGDE_GUI_APP_KEY_HANDLER_H_
#define MENGDE_GUI_APP_KEY_HANDLER_H_

#include <functional>
#include <memory>
#include <type_traits>

#include "key_mapper.h"

namespace mengde {
namespace gui {
namespace app {

class KeyHandler {
 public:
  using KeyCallback = std::function<void()>;

 public:
  enum KeyType { kNone, kUp, kDown };

  struct Event {
    KeyCmdCode code;
    KeyType type;

    bool operator==(const Event& o) const { return code == o.code && type == o.type; }
  };

  struct EventHasher {
    std::size_t operator()(const Event& e) const {
      using KeyCmdCodeUnderlying = std::underlying_type<KeyCmdCode>::type;
      return std::hash<KeyCmdCodeUnderlying>()(static_cast<KeyCmdCodeUnderlying>(e.code));
    }
  };

 public:
  KeyHandler() = default;

 public:
  void Register(Event event, std::unique_ptr<KeyCallback>&& callback);
  void Unregister(Event event);
  void Run(Event event);

 private:
  std::unordered_map<Event, std::unique_ptr<KeyCallback>, EventHasher> map_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_KEY_HANDLER_H_
