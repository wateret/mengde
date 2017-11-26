#ifndef TIMER_H_
#define TIMER_H_

#include "common.h"

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

#endif
