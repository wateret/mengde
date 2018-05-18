#ifndef MENGDE_GUI_FOUNDATION_KEY_EVENT_H_
#define MENGDE_GUI_FOUNDATION_KEY_EVENT_H_

#include "util/common.h"

namespace mengde {
namespace gui {
namespace foundation {

class KeyEvent {
 public:
  enum class State : uint16_t { kIdle, kDown, kUp };
  enum class Mod : uint16_t { kNone = 0, kCtrl = 1, kShift = 2, kAlt = 4 };

 public:
  KeyEvent(const uint32_t code, const Mod mod, const State state,
           uint8_t repeat, uint32_t timestamp);

 public:
  uint32_t GetCode() const { return code_; }
  Mod GetMod() const { return mod_; }
  State GetState() const { return state_; }
  uint8_t GetRepeat() const { return repeat_; }
  uint32_t GetTimestamp() const { return timestamp_; }

 private:
  uint32_t code_;
  Mod mod_;
  State state_;
  uint8_t repeat_;
  uint32_t timestamp_;
};

inline KeyEvent::Mod operator|(KeyEvent::Mod a, KeyEvent::Mod b) {
  return static_cast<KeyEvent::Mod>
            (static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
}

}  // namespace foundation
}  // namespace gui
}  // namespace mengde

#endif  // MENGDE_GUI_FOUNDATION_KEY_EVENT_H_
