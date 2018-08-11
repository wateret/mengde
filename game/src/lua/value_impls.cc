#include "value_impls.h"

namespace lua {

template <typename T>
ValueImpls<T>::ValueImpls(const T& value) : value_(value) {}

template <>
ValueImpl* ValueImpls<Table>::New() {
  printf("Table not really create a new object\n");
  // For table do not generate a new object just copy the pointer
  return this;
}

template class ValueImpls<int32_t>;
template class ValueImpls<double>;
template class ValueImpls<std::string>;
template class ValueImpls<Table>;
template class ValueImpls<void*>;

}  // namespace lua
