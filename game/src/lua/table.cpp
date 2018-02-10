#include "table.h"

namespace lua {

Table::~Table() {
  for (auto e : values_) {
    delete e.second;
  }
}

void Table::Dump() {
#ifdef DEBUG
  printf("Table {\n");
  for (auto e : values_) {
    auto& key = e.first;
    auto value = e.second;
    printf("  %s: ", key.c_str());
    switch (value->type()) {
      case Value::Type::kInt32:
        printf("%d", value->Get<int32_t>());
        break;

      case Value::Type::kString:
        printf("\"%s\"", value->Get<std::string>().c_str());
        break;

      default:
        printf("[some value]");
        break;
    }
    printf("\n");
  }
  printf("}\n");
#endif
}

} // namespace lua
