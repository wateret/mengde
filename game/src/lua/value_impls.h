#ifndef LUA_VALUE_IMPLS_H_
#define LUA_VALUE_IMPLS_H_

#include "value_impl.h"
#include "table.h"

namespace lua {

template <typename T>
class ValueImpls : public ValueImpl {
 public:
  ValueImpls(const T& value);
  virtual ~ValueImpls() = default;

 public:
  ValueImpls(const ValueImpls& value) = delete;
  ValueImpls(ValueImpls&& value) = delete;

 public:
  virtual ValueImpl* New() {
    return new ValueImpls{value_};
  }

 public:
  T GetValue() const { return value_; }

 private:
  T value_;
};

template <>
ValueImpl* ValueImpls<Table>::New();

}  // namespace lua

#endif  // LUA_VALUE_IMPLS_
