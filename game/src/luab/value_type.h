#ifndef LUAB_VALUE_TYPE_H_
#define LUAB_VALUE_TYPE_H_

namespace luab {

enum class ValueType {
  kNone,
  kNumber,
  kString,
  kTable,
  kUserdata,
};

}  // namespace luab

#endif  // LUAB_VALUE_TYPE_H_
