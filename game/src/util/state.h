#ifndef UTIL_STATE_H_
#define UTIL_STATE_H_

#include <string>

class State {
 public:
  virtual ~State() {}
  virtual void Enter() = 0;
  virtual void Exit()  = 0;
#ifdef DEBUG
  virtual std::string GetStateID() const = 0;
#endif
};

#endif  // UTIL_STATE_H_
