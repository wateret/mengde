#include "stat.h"

namespace mengde {
namespace core {

Attribute operator+(const Attribute& lhs, const Attribute& rhs) {
  return {lhs.atk + rhs.atk,
          lhs.def + rhs.def,
          lhs.dex + rhs.dex,
          lhs.itl + rhs.itl,
          lhs.mor + rhs.mor};
}

Attribute operator+(const Attribute& lhs, int rhs) {
  return {lhs.atk + rhs,
          lhs.def + rhs,
          lhs.dex + rhs,
          lhs.itl + rhs,
          lhs.mor + rhs};
}

Attribute& operator+=(Attribute& lhs, const Attribute& rhs) {
  lhs.atk += rhs.atk;
  lhs.def += rhs.def;
  lhs.dex += rhs.dex;
  lhs.itl += rhs.itl;
  lhs.mor += rhs.mor;
  return lhs;
}

Attribute& operator*=(Attribute& lhs, const Attribute& rhs) {
  lhs.atk *= rhs.atk;
  lhs.def *= rhs.def;
  lhs.dex *= rhs.dex;
  lhs.itl *= rhs.itl;
  lhs.mor *= rhs.mor;
  return lhs;
}

Attribute& operator/=(Attribute& lhs, int rhs) {
  lhs.atk /= rhs;
  lhs.def /= rhs;
  lhs.dex /= rhs;
  lhs.itl /= rhs;
  lhs.mor /= rhs;
  return lhs;
}

} // namespace core
} // namespace mengde
