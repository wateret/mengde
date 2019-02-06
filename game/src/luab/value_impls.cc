#include "value_impls.h"

namespace luab {

using Userdata = void*;

template <>
ValueImpls<double>::ValueImpls(const double& value) : ValueImpl{ValueType::kNumber}, value_{value} {}

template <>
ValueImpls<std::string>::ValueImpls(const std::string& value) : ValueImpl{ValueType::kString}, value_{value} {}

template <>
ValueImpls<Table>::ValueImpls(const Table& value) : ValueImpl{ValueType::kTable}, value_{value} {}

template <>
ValueImpls<Userdata>::ValueImpls(const Userdata& value) : ValueImpl{ValueType::kUserdata}, value_{value} {}

template <>
ValueImpl* ValueImpls<Table>::New() {
  // For table do not generate a new object just copy the pointer
  return this;
}

template class ValueImpls<double>;
template class ValueImpls<std::string>;
template class ValueImpls<Table>;
template class ValueImpls<Userdata>;

}  // namespace luab
