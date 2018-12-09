#ifndef MENGDE_CORE_ID_H_
#define MENGDE_CORE_ID_H_

#include <boost/optional.hpp>
#include <utility>

namespace mengde {
namespace core {

template <typename T, typename Tag>
class Id {
 public:
  Id() : val_{boost::none} {}
  Id(T&& o) : val_{std::move(o)} {}
  Id(const T& o) : val_{o} {}
  Id(Id<T, Tag>&& o) : val_{std::move(o.val_)} {}
  Id(const Id<T, Tag>& o) : val_{o.val_} {}

  Id<T, Tag>& operator=(T&& o) {
    val_ = std::move(o);
    return *this;
  }

  Id<T, Tag>& operator=(const T& o) {
    val_ = o;
    return *this;
  }

  Id<T, Tag>& operator=(Id<T, Tag>&& o) {
    val_ = std::move(o.val_);
    return *this;
  }

  Id<T, Tag>& operator=(const Id<T, Tag>& o) {
    val_ = o.val_;
    return *this;
  }

  bool operator==(const T& o) const { return val_ == o; }
  bool operator==(const Id<T, Tag>& o) const { return val_ == o.val_; }
  bool operator!=(const T& o) const { return !(*this == o); }
  bool operator!=(const Id<T, Tag>& o) const { return !(*this == o); }

  bool operator!() const { return IsNone(); }
  explicit operator bool() const { return !IsNone(); }

  void SetNone() { val_ = boost::none; }
  bool IsNone() const { return val_ == boost::none; }
  T Value() const { return val_.get(); }

 private:
  boost::optional<T> val_;
};

using UId = Id<uint32_t, struct Tag_UId>;
using UnitKey = Id<uint32_t, struct Tag_UnitKey>;
using MoveKey = Id<uint32_t, struct Tag_MoveKey>;
using ActKey = Id<uint32_t, struct Tag_ActKey>;

}  // namespace core
}  // namespace mengde

// Specialization of std::hash for class Id

namespace std {

template <typename T, typename Tag>
struct hash<::mengde::core::Id<T, Tag>> {
  typedef ::mengde::core::Id<T, Tag> ArgType;
  size_t operator()(const ArgType& arg) const {
    // If the value is None return 0, otherwise return the hash value of the underlying value
    return arg ? std::hash<T>{}(arg.Value()) : 0u;
  }
};

}  // namespace std

#endif  // MENGDE_CORE_ID_H_
