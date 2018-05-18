#include "util/common.h"
#include "key_event.h"

namespace mengde {
namespace gui {
namespace foundation {

KeyEvent::KeyEvent(const uint32_t code, const Mod mod, const State state,
                   uint8_t repeat, uint32_t timestamp)
    : code_(code), mod_(mod), state_(state), repeat_(repeat), timestamp_(timestamp) {}

}  // namespace foundation
}  // namespace gui
}  // namespace mengde
