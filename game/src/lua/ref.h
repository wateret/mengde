#ifndef LUA_REF_H_
#define LUA_REF_H_

extern "C" {

#include "lauxlib.h"
}

namespace lua {

class Ref {
 public:
  Ref(void) : index_{LUA_REFNIL} {}
  Ref(int index) : index_{index} {}
  int value() const { return index_; }

 public:
  bool nil() const { return index_ == LUA_REFNIL; }

 private:
  int index_;
};

}  // namespace lua

#endif  // LUA_REF_H_
