#include "value.h"

#include <string.h>

#include "value_impls.h"
#include "table.h"

namespace lua {

//
// Value
//

ValueImpl* Value::operator->() noexcept { return value_.operator->(); }

ValueImpl& Value::operator*() noexcept { return value_.operator*(); }

Value::Value(const Value& value) {
  value_ = ValueImpl::New(value.value_);
}

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
  if (dynamic_cast<ValueImpls<int32_t>*>(ptr)) {
    return Type::kInt32;
  } else if (dynamic_cast<ValueImpls<double>*>(ptr)) {
    return Type::kDouble;
  } else if (dynamic_cast<ValueImpls<std::string>*>(ptr)) {
    return Type::kString;
  } else if (dynamic_cast<ValueImpls<Table>*>(ptr)) {
    return Type::kTable;
  } else if (dynamic_cast<ValueImpls<void*>*>(ptr)) {
    return Type::kUserdata;
  }
  assert(!"Unknown type of value");
}

using Userdata = void*;

template Value::Value(const int32_t& value);

template Value::Value(const double& value);

template Value::Value(const std::string& value);

template Value::Value(const Table& value);

template Value::Value(const Userdata& value);


template int32_t Value::Get<int32_t>() const;

template double Value::Get<double>() const;

template std::string Value::Get<std::string>() const;

template Table Value::Get<Table>() const;

template Userdata Value::Get<Userdata>() const;

}  // namespace lua
