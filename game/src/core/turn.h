#ifndef TURN_H_
#define TURN_H_

#include "unit.h"

class Turn {
 public:
  Turn();
  bool Next();
  uint16_t GetCurrent() const { return current_; }
  uint16_t GetLimit() const { return limit_; }
  Unit::Side GetSide() const { return side_; }

 private:
  uint16_t   current_;
  uint16_t   limit_;
  Unit::Side side_;
};

#endif // TURN_H_
