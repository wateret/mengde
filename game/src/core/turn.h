#ifndef TURN_H_
#define TURN_H_

#include "unit.h"

class Turn {
 public:
  Turn();
  bool Next();
  Unit::Side GetSide() const { return side_; }

 private:
  int        no_;
//int        limit_;
  Unit::Side side_;
};

#endif // TURN_H_
