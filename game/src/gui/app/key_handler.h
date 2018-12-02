#ifndef MENGDE_GUI_APP_KEY_HANDLER_H_
#define MENGDE_GUI_APP_KEY_HANDLER_H_

#include <functional>

#include "key_mapper.h"

namespace mengde {
namespace gui {
namespace app {

class KeyHandler {
 public:
  using KeyCallback = std::function<void()>;

 public:
  KeyHandler() = default;

 public:
  void Register(KeyCmdCode code, std::unique_ptr<KeyCallback>&& callback);
  void Unregister(KeyCmdCode code);
  void Run(KeyCmdCode code);

 private:
  std::unordered_map<KeyCmdCode, std::unique_ptr<KeyCallback>> map_;
};

}  // namespace app
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_APP_KEY_HANDLER_H_
