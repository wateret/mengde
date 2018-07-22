#ifndef MENGDE_CORE_ID_H_
#define MENGDE_CORE_ID_H_

#include <boost/optional.hpp>

namespace mengde {
namespace core {

template <typename T>
class Id {
 public:
  Id() : val_{boost::none} {}
  Id(T&& o) : val_{std::forward<T>(o)} {}
  Id(Id<T>&& o) : val_{std::move<Id<T>>(o.val_)} {}
  Id(const Id<T>& o) : val_{o.val_} {}

  Id<T>& operator=(T&& o) {
    val_ = std::forward<T>(o);
    return *this;
  }

  Id<T>& operator=(Id<T>&& o) {
    val_ = std::move<Id<T>>(o.val_);
    return *this;
  }

  Id<T>& operator=(const Id<T>& o) {
    val_ = o.val_;
    return *this;
  }

  bool operator==(const T& o) const { return val_ == o; }
  bool operator==(const Id<T>& o) const { return val_ == o.val_; }
  bool operator!=(const T& o) const { return !(*this == o); }
  bool operator!=(const Id<T>& o) const { return !(*this == o); }

  bool operator!() { return IsNone(); }
  operator bool() const { return !IsNone(); }

  void SetNone() { val_ = boost::none; }
  bool IsNone() const { return val_ == boost::none; }
  T Value() const { return val_.get(); }

 private:
  boost::optional<T> val_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_ID_H_
