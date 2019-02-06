#include "table.h"

namespace luab {

/*
TableImpl::TableImpl(const TableImpl& table) {
  for (auto e : table.values_) {
    values_.insert({e.first, new Value(*e.second)});
  }
}
*/

void TableImpl::ForEachNonArray(const std::function<void(const std::string& key, const luab::Value& val)>& fn) const {
  for (auto e : values_) {
    fn(e.first, e.second);
  }
}

void TableImpl::Dump() {
  printf("Table {\n");
  for (auto e : values_) {
    auto& key = e.first;
    auto value = e.second;
    printf("  %s: ", key.c_str());
    switch (value.type()) {
      case Value::Type::kNumber:
        printf("%f", value.Get<double>());
        break;

      case Value::Type::kString:
        printf("\"%s\"", value.Get<std::string>().c_str());
        break;

      default:
        printf("[some value]");
        break;
    }
    printf("\n");
  }
  printf("}\n");
}

Table::Table() : table_(std::make_shared<TableImpl>()) {}

void Table::ForEachNonArray(const std::function<void(const std::string&, const luab::Value&)>& fn) const {
  table_->ForEachNonArray(fn);
}

bool Table::empty() const { return table_->empty(); }

}  // namespace luab
