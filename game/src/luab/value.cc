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

ValueType Value::type() const { return value_->type(); }

using Userdata = void*;

template Value::Value(const double& value);

template <>
Value::Value(const uint16_t& value) : Value{static_cast<double>(value)} {}

template <>
Value::Value(const int16_t& value) : Value{static_cast<double>(value)} {}

template <>
Value::Value(const uint32_t& value) : Value{static_cast<double>(value)} {}

template <>
Value::Value(const int32_t& value) : Value{static_cast<double>(value)} {}

template Value::Value(const std::string& value);

template Value::Value(const Table& value);

template Value::Value(const Userdata& value);

template double Value::Get<double>() const;

template <>
uint16_t Value::Get() const {
  // TODO Check if in valid range
  return static_cast<uint16_t>(Get<double>());
}

template <>
int16_t Value::Get() const {
  // TODO Check if in valid range
  return static_cast<int16_t>(Get<double>());
}

template <>
uint32_t Value::Get() const {
  // TODO Check if in valid range
  return static_cast<uint32_t>(Get<double>());
}

template <>
int32_t Value::Get() const {
  // TODO Check if in valid range
  return static_cast<int32_t>(Get<double>());
}

template std::string Value::Get<std::string>() const;

template Table Value::Get<Table>() const;

template Userdata Value::Get<Userdata>() const;

}  // namespace luab
