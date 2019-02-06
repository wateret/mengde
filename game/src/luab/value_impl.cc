#include "value_impl.h"

#include "exceptions.h"
#include "value_impls.h"

namespace luab {

// Generate a new ValueImpl from the argument value
// Depend on the type, the underlying value is copied or can be just poniter is copied.

std::shared_ptr<ValueImpl> ValueImpl::New(const std::shared_ptr<ValueImpl>& value) {
  auto new_value = value->New();
  if (new_value == value.get()) {
    // If it returns the same object, just return the value
    return value;
  } else {
    // Otherwise make a new object as shared
    return std::shared_ptr<ValueImpl>(new_value);
  }
}

template <typename T>
T ValueImpl::Get() const {
  auto actual_this = dynamic_cast<const ValueImpls<T>*>(this);
  assert(actual_this != nullptr);
  if (actual_this == nullptr) {
    throw WrongTypeException{"?", "?"};  // TODO Replace question marks with real type names
  }
  return actual_this->GetValue();
}

template double ValueImpl::Get<double>() const;
template std::string ValueImpl::Get<std::string>() const;
template Table ValueImpl::Get<Table>() const;
template void* ValueImpl::Get<void*>() const;

}  // namespace luab
