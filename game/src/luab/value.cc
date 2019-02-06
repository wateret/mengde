#include "value.h"

#include <string.h>

#include "table.h"
#include "value_impls.h"

namespace luab {

//
// Value
//

ValueImpl* Value::operator->() noexcept { return value_.operator->(); }

ValueImpl& Value::operator*() noexcept { return value_.operator*(); }

Value::Value(const Value& value) { value_ = ValueImpl::New(value.value_); }

template <typename T>
Value::Value(const T& value) {
  value_ = std::make_shared<ValueImpls<T>>(value);
}

template <typename T>
T Value::Get() const {
  return value_->Get<T>();
}

Value::Type Value::type() const {
  auto ptr = value_.get();
  if (dynamic_cast<ValueImpls<double>*>(ptr)) {
    return Type::kNumber;
  } else if (dynamic_cast<ValueImpls<std::string>*>(ptr)) {
    return Type::kString;
  } else if (dynamic_cast<ValueImpls<Table>*>(ptr)) {
    return Type::kTable;
  } else if (dynamic_cast<ValueImpls<void*>*>(ptr)) {
    return Type::kUserdata;
  }
  assert(!"Unknown type of value");
  return Type::kNone;
}

using Userdata = void*;

template Value::Value(const double& value);

template <>
Value::Value(const int32_t& value) : Value{static_cast<double>(value)} {}

template Value::Value(const std::string& value);

template Value::Value(const Table& value);

template Value::Value(const Userdata& value);

template double Value::Get<double>() const;

template <>
int32_t Value::Get() const {
  // TODO Check if in valid range
  return static_cast<int32_t>(Get<double>());
}

template std::string Value::Get<std::string>() const;

template Table Value::Get<Table>() const;

template Userdata Value::Get<Userdata>() const;

}  // namespace luab
