#ifndef MENGDE_UTIL_STD_UTIL_H_
#define MENGDE_UTIL_STD_UTIL_H_

#include <vector>

namespace mengde {
namespace util {
namespace std {

template <class T, class UnaryPredicate>
void VectorEraseIf(::std::vector<T>& v, UnaryPredicate pred) {
  v.erase(remove_if(v.begin(), v.end(), pred), v.end());
}

}  // namespace std
}  // namespace util
}  // namespace mengde

#endif  // MENGDE_UTIL_STD_UTIL_H_
