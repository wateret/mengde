#ifndef TIMER_H_
#define TIMER_H_

#include "util/common.h"

namespace mengde {
namespace gui {
namespace foundation {

class Timer {
 public:
  Timer();

 public:
  void Start();
  int Stop();
  int Split();

 private:
  bool is_on_;
  uint32_t start_ticks_;
};

} // namespace foundation
} // namespace gui
} // namespace mengde

#endif
