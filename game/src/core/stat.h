#ifndef STAT_H_
#define STAT_H_

#include <stdint.h>

namespace mengde {
namespace core {

struct Attribute {
  enum { kStatIdAtk, kStatIdDef, kStatIdDex, kStatIdItl, kStatIdMor };

  int atk;
  int def;
  int dex;
  int itl;
  int mor;

  Attribute() : atk(0), def(0), dex(0), itl(0), mor(0) {}

  Attribute(int atk, int def, int dex, int itl, int mor) : atk(atk), def(def), dex(dex), itl(itl), mor(mor) {}

  void ApplyModifier(const Attribute& addend, const Attribute& multiplier);

  int GetValueByIndex(int index) const {
    // Fragile but fast implementation
    return *((int*)(this) + index);
  }

  void AddValueByIndex(int index, int value) {
    // Fragile but fast implementation
    ((int*)(this))[index] += value;
  }

  void SetValueByIndex(int index, int value) {
    // Fragile but fast implementation
    ((int*)(this))[index] = value;
  }
};

#define ATTRIBUTE_BIN_OP_DECL(OP) Attribute operator OP(const Attribute& lhs, const Attribute& rhs);

#define ATTRIBUTE_BIN_OP_NUMERIC_DECL(OP, TYPE)          \
  Attribute operator OP(const Attribute& lhs, TYPE rhs); \
  Attribute operator OP(TYPE lhs, const Attribute& rhs);

#define ATTRIBUTE_ASSIGN_OP_DECL(OP) Attribute& operator OP(Attribute& lhs, const Attribute& rhs);

#define ATTRIBUTE_ASSIGN_OP_NUMERIC_DECL(OP, TYPE) Attribute& operator OP(Attribute& lhs, TYPE rhs);

#define ATTRIBUTE_COMPARATOR_NUMERIC_DECL(OP, TYPE) bool operator OP(const Attribute& lhs, TYPE rhs);

ATTRIBUTE_BIN_OP_DECL(+)
ATTRIBUTE_BIN_OP_DECL(-)
ATTRIBUTE_BIN_OP_DECL(*)
ATTRIBUTE_BIN_OP_DECL(/)

ATTRIBUTE_BIN_OP_NUMERIC_DECL(+, int)
ATTRIBUTE_BIN_OP_NUMERIC_DECL(-, int)
ATTRIBUTE_BIN_OP_NUMERIC_DECL(*, int)
ATTRIBUTE_BIN_OP_NUMERIC_DECL(/, int)

ATTRIBUTE_ASSIGN_OP_DECL(+=)
ATTRIBUTE_ASSIGN_OP_DECL(-=)
ATTRIBUTE_ASSIGN_OP_DECL(*=)
ATTRIBUTE_ASSIGN_OP_DECL(/=)

ATTRIBUTE_ASSIGN_OP_NUMERIC_DECL(+=, int)
ATTRIBUTE_ASSIGN_OP_NUMERIC_DECL(-=, int)
ATTRIBUTE_ASSIGN_OP_NUMERIC_DECL(*=, int)
ATTRIBUTE_ASSIGN_OP_NUMERIC_DECL(/=, int)

ATTRIBUTE_COMPARATOR_NUMERIC_DECL(==, int)
ATTRIBUTE_COMPARATOR_NUMERIC_DECL(<, int)
ATTRIBUTE_COMPARATOR_NUMERIC_DECL(>, int)
ATTRIBUTE_COMPARATOR_NUMERIC_DECL(<=, int)
ATTRIBUTE_COMPARATOR_NUMERIC_DECL(<=, int)

#undef ATTRIBUTE_BIN_OP_DECL
#undef ATTRIBUTE_BIN_OP_NUMERIC_DECL
#undef ATTRIBUTE_ASSIGN_OP_DECL
#undef ATTRIBUTE_ASSIGN_OP_NUMERIC_DECL
#undef ATTRIBUTE_COMPARATOR_NUMERIC_DECL

struct HpMp {
  int hp;
  int mp;

  HpMp() : hp(0), mp(0) {}

  HpMp(int hp, int mp) : hp(hp), mp(mp) {}
};

struct Level {
  static const uint16_t kExpLimit = 100;

  uint16_t level;
  uint16_t exp;

  Level(uint16_t level, uint16_t exp) : level(level), exp(exp) {}
};

}  // namespace core
}  // namespace mengde

#endif  // STAT_H_
