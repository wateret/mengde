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
    printf("  %s: ", key.c_str());
    printf("value");
    printf("\n");
  }
  printf("}\n");
#endif
}

} // namespace lua
