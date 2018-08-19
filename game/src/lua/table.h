#ifndef LUA_TABLE_H_
#define LUA_TABLE_H_

#include <functional>
#include <memory>
#include <string>

#include "value.h"

namespace lua {

class TableImpl {
 public:
  TableImpl(const TableImpl&) = delete;
  TableImpl(TableImpl&&) = delete;

 public:
  TableImpl() = default;
  ~TableImpl() = default;

 public:
  template <typename T>
  void Set(const std::string& key, T value) {
    values_.insert({key, Value(value)});
  }

  template <typename T>
  T Get(const std::string& key) const {
    auto found = values_.find(key);
    if (found == values_.end()) {
      throw "Given key '" + key + "' does not exist in TableImpl.";
    }
    return found->second.Get<T>();
  }

  template <typename T>
  T Get(const std::string& key, T default_value) const {
    auto found = values_.find(key);
    if (found == values_.end()) {
      return default_value;
    }
    return found->second.Get<T>();
  }

  void ForEachNonArray(const std::function<void(const std::string&, const lua::Value&)>& fn) const;

  bool empty() const { return values_.empty(); }

  void Dump();

 private:
  // TODO The key must be Value not just string.
  std::unordered_map<std::string, Value> values_;
};

class Table {
 public:
  Table();

 public:
  Table(const Table& table) = default;
  Table(Table&& table) = default;

 public:
  template <typename T>
  void Set(const std::string& key, T value) {
    table_->Set(key, value);
  }

  template <typename T>
  T Get(const std::string& key) const {
    return table_->Get<T>(key);
  }

  template <typename T>
  T Get(const std::string& key, T default_value) const {
    return table_->Get<T>(key, default_value);
  }

  void ForEachNonArray(const std::function<void(const std::string&, const lua::Value&)>& fn) const;

 public:
  bool empty() const;

 public:
  TableImpl* operator->() noexcept { return table_.operator->(); }
  TableImpl& operator*() noexcept { return table_.operator*(); }

 private:
  std::shared_ptr<TableImpl> table_;
};

}  // namespace lua

#endif  // LUA_TABLE_H_
