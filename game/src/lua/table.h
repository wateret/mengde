#ifndef LUA_LUA_TABLE_H_
#define LUA_LUA_TABLE_H_

#include "value.h"

namespace lua {

class Table {
 public:
  Table() = default;
  ~Table();

  template <typename T>
  void Add(const std::string& key, T value) {
    values_.insert({key, new Value(value)});
  }

  template <typename T>
  T Get(const std::string& key) const {
    auto found = values_.find(key);
    if (found == values_.end()) {
      throw "Given key '" + key + "' does not exist in Table.";
    }
    return found->second->Get<T>();
  }

  template <typename T>
  T Get(const std::string& key, T default_value) const {
    auto found = values_.find(key);
    if (found == values_.end()) {
      return default_value;
    }
    return found->second->Get<T>();
  }

  void Dump();

 private:
  std::unordered_map<std::string, Value*> values_;
};

}  // namespace lua

#endif  // LUA_LUA_TABLE_H_
