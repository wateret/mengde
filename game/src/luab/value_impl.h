#ifndef LUA_VALUE_IMPL_H_
#define LUA_VALUE_IMPL_H_

#include <memory>

#include "value_type.h"

namespace luab {

//
// ValueImpl is a generic type for lua values
//

class ValueImpl {
 public:
  static std::shared_ptr<ValueImpl> New(const std::shared_ptr<ValueImpl>& value);

 public:
  ValueImpl(ValueType type = ValueType::kNone) : type_{type} {}

 public:
  virtual ~ValueImpl() = default;
  virtual ValueImpl* New() = 0;

 public:
  template <typename T>
  T Get() const;

  ValueType type() const { return type_; }

 private:
  ValueType type_;
};

}  // namespace luab

#endif  // LUA_VALUE_IMPL_H_
