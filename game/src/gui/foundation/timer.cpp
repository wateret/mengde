#include "timer.h"
#include <SDL.h>


namespace mengde {
namespace gui {
namespace foundation {

Timer::Timer() : is_on_(false), start_ticks_(0) {
}


void Timer::Start() {
  is_on_ = true;
  start_ticks_ = SDL_GetTicks();
}


int Timer::Stop() {
  int ret = Split();
  is_on_ = false;
  return ret;
}


int Timer::Split() {
  ASSERT(is_on_);
  uint32_t cur_ticks = SDL_GetTicks();
  return (int)(cur_ticks - start_ticks_);
}

} // namespace foundation
} // namespace gui
} // namespace mengde
