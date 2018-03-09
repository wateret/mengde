#ifndef LUA_VALUE_H_
#define LUA_VALUE_H_

#include <assert.h>
#include <stdint.h>

#include <string>
#include <unordered_map>

namespace lua {

class Table;

// Value is a generic type for lua values
class Value {
 public:
  enum class Type {
    kNone,
    kInt32,
    kDouble,
    kString,
    kTable,
    kUserdata,
  };

 public:
  Value(const Value& lua_value);
  Value(Value&& lua_value);

  Value(int32_t value);
  Value(double value);
  Value(const std::string& value);
  Value(const Table& value);
  Value(void* value);
  ~Value();

  Type type() { return type_; }

  template <typename T>
  T Get() const {
    assert(false && "Unreachable code");
    return 0;
  }

 private:
  Type type_;
  // FIXME Union is not a good way to implement any type
  union U {
    int32_t     v_int32;
    double      v_double;
    std::string v_string;
    Table*      v_table;
    void*       v_userdata;

    U() {}
    ~U() {}  // For non-POD types the destructor will be called from Value destructor
  } value_;
};

template <>
int32_t Value::Get<int32_t>() const;

template <>
double Value::Get<double>() const;

template <>
std::string Value::Get<std::string>() const;

template <>
Table* Value::Get<Table*>() const;

template <>
void* Value::Get<void*>() const;

}  // namespace lua

#endif  // LUA_VALUE_H_
