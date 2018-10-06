#ifndef MENGDE_CORE_STAT_H_
#define MENGDE_CORE_STAT_H_

#include <stdint.h>

namespace mengde {
namespace core {

struct Attribute {
  static const char* kToString[];

  int atk;
  int def;
  int dex;
  int itl;
  int mor;

  Attribute() : Attribute{0, 0, 0, 0, 0} {}

  Attribute(int atk, int def, int dex, int itl, int mor) : atk{atk}, def{def}, dex{dex}, itl{itl}, mor{mor} {}

  void ApplyModifier(const Attribute& addend, const Attribute& multiplier);

  int& operator[](uint32_t index) { return *((int*)(this) + index); }

  int operator[](uint32_t index) const { return *((int*)(this) + index); }
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
  static const uint16_t kExpLimit = 200;

  uint16_t level;
  uint16_t exp;

  Level(uint16_t level, uint16_t exp) : level(level), exp(exp) {}
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_STAT_H_
