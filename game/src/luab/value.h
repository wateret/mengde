#ifndef LUA_VALUE_H_
#define LUA_VALUE_H_

#include <assert.h>
#include <stdint.h>

#include <memory>
#include <string>
#include <unordered_map>

#include "value_type.h"

namespace luab {

//
// Value is a wrapper of ValueImpl to have data as shared_ptr inside
//

class ValueImpl;

class Value {
 public:
  Value() = delete;

 public:
  Value(const Value& value);

  template <typename T>
  Value(const T& value);

  ~Value() = default;

 public:
  ValueImpl* operator->() noexcept;
  ValueImpl& operator*() noexcept;

 public:
  ValueType type() const;

 public:
  template <typename T>
  T Get() const;

 private:
  std::shared_ptr<ValueImpl> value_;
};

}  // namespace luab

#endif  // LUA_VALUE_H_
