#ifndef LUA_VALUE_IMPL_H_
#define LUA_VALUE_IMPL_H_

#include <memory>

namespace lua {

//
// ValueImpl is a generic type for lua values
//

class ValueImpl {
 public:
  static std::shared_ptr<ValueImpl> New(const std::shared_ptr<ValueImpl>& value);

 public:
  virtual ~ValueImpl() = default;
  virtual ValueImpl* New() = 0;

 public:
  template <typename T>
  T Get() const;
};

}  // namespace lua

#endif  // LUA_VALUE_IMPL_H_
