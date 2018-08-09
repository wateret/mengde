#include "value.h"

#include <string.h>

#include "table.h"

namespace lua {

Value::Value(const Value& lua_value) {
  memcpy(static_cast<void*>(this), static_cast<const void*>(&lua_value), sizeof(Value));
  assert(type_ == lua_value.type_);
}

Value::Value(int32_t value) {
  type_ = Type::kInt32;
  value_.v_int32 = value;
}

Value::Value(double value) {
  type_ = Type::kDouble;
  value_.v_double = value;
}

Value::Value(const std::string& value) {
  type_ = Type::kString;
  new ((void*)&value_.v_string) std::string(value);
}

Value::Value(const Table& value) {
  type_ = Type::kTable;
  value_.v_table = new Table(value);
}

Value::Value(void* value) {
  type_ = Type::kUserdata;
  value_.v_userdata = value;
}

Value::~Value() {
  switch (type_) {
    case Type::kString:
      using std::string;
      value_.v_string.~string();
      break;

    case Type::kTable:
      delete value_.v_table;
      break;

    case Type::kInt32:
    case Type::kDouble:
    case Type::kUserdata:
      // Nothing to destroy for POD types
      break;

    default:
      assert(false && "Unreachable");
      break;
  }
}

// Template specializations

template <>
int32_t Value::Get<int32_t>() const {
  assert(type_ == Type::kInt32);
  return value_.v_int32;
}

template <>
double Value::Get<double>() const {
  assert(type_ == Type::kDouble);
  return value_.v_double;
}

template <>
std::string Value::Get<std::string>() const {
  assert(type_ == Type::kString);
  return value_.v_string;
}

template <>
Table* Value::Get<Table*>() const {
  assert(type_ == Type::kTable);
  return value_.v_table;
}

template <>
void* Value::Get<void*>() const {
  assert(type_ == Type::kUserdata);
  return value_.v_userdata;
}

}  // namespace lua
