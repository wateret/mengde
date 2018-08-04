#ifndef LUA_REF_H_
#define LUA_REF_H_

namespace lua {

class Ref {
 public:
  Ref(int index) : index_{index} {}
  int value() const { return index_; }

 private:
  int index_;
};

}  // namespace lua

#endif  // LUA_REF_H_
