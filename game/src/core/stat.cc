#include "stat.h"

#include "util/common.h"

namespace mengde {
namespace core {

const char* Attribute::kToString[] = {"atk", "def", "dex", "itl", "mor", nullptr};

void Attribute::ApplyModifier(const Attribute& addend, const Attribute& multiplier) {
  ASSERT(multiplier > -100);

  Attribute& object = *this;

  // Add addend first then multiply multiplier
  object += addend;
  object *= multiplier + 100;
  object /= 100;

  /* TODO We may need to limit the calculated attribute to be equal or bigger than 0
  object.atk = std::max(0, object.atk);
  object.def = std::max(0, object.def);
  object.dex = std::max(0, object.dex);
  object.itl = std::max(0, object.itl);
  object.mor = std::max(0, object.mor);
  */
}

#define ATTRIBUTE_BIN_OP_DEF(OP)                                                                                 \
  Attribute operator OP(const Attribute& lhs, const Attribute& rhs) {                                            \
    return {lhs.atk OP rhs.atk, lhs.def OP rhs.def, lhs.dex OP rhs.dex, lhs.itl OP rhs.itl, lhs.mor OP rhs.mor}; \
  }

#define ATTRIBUTE_BIN_OP_NUMERIC_DEF(OP, TYPE)                                               \
  Attribute operator OP(const Attribute& lhs, TYPE rhs) {                                    \
    return {lhs.atk OP rhs, lhs.def OP rhs, lhs.dex OP rhs, lhs.itl OP rhs, lhs.mor OP rhs}; \
  }                                                                                          \
                                                                                             \
  Attribute operator OP(TYPE lhs, const Attribute& rhs) {                                    \
    return {lhs OP rhs.atk, lhs OP rhs.def, lhs OP rhs.dex, lhs OP rhs.itl, lhs OP rhs.mor}; \
  }

#define ATTRIBUTE_ASSIGN_OP_DEF(OP)                              \
  Attribute& operator OP(Attribute& lhs, const Attribute& rhs) { \
    lhs.atk OP rhs.atk;                                          \
    lhs.def OP rhs.def;                                          \
    lhs.dex OP rhs.dex;                                          \
    lhs.itl OP rhs.itl;                                          \
    lhs.mor OP rhs.mor;                                          \
    return lhs;                                                  \
  }

#define ATTRIBUTE_ASSIGN_OP_NUMERIC_DEF(OP, TYPE)    \
  Attribute& operator OP(Attribute& lhs, TYPE rhs) { \
    lhs.atk OP rhs;                                  \
    lhs.def OP rhs;                                  \
    lhs.dex OP rhs;                                  \
    lhs.itl OP rhs;                                  \
    lhs.mor OP rhs;                                  \
    return lhs;                                      \
  }

#define ATTRIBUTE_COMPARATOR_NUMERIC_DEF(OP, TYPE)                                                 \
  bool operator OP(const Attribute& lhs, TYPE rhs) {                                               \
    return lhs.atk OP rhs && lhs.def OP rhs && lhs.dex OP rhs && lhs.itl OP rhs && lhs.mor OP rhs; \
  }                                                                                                \
                                                                                                   \
  bool operator OP(TYPE lhs, const Attribute& rhs) {                                               \
    return lhs OP rhs.atk && lhs OP rhs.def && lhs OP rhs.dex && lhs OP rhs.itl && lhs OP rhs.mor; \
  }

ATTRIBUTE_BIN_OP_DEF(+)
ATTRIBUTE_BIN_OP_DEF(-)
ATTRIBUTE_BIN_OP_DEF(*)
ATTRIBUTE_BIN_OP_DEF(/)

ATTRIBUTE_BIN_OP_NUMERIC_DEF(+, int)
ATTRIBUTE_BIN_OP_NUMERIC_DEF(-, int)
ATTRIBUTE_BIN_OP_NUMERIC_DEF(*, int)
ATTRIBUTE_BIN_OP_NUMERIC_DEF(/, int)

ATTRIBUTE_ASSIGN_OP_DEF(+=)
ATTRIBUTE_ASSIGN_OP_DEF(-=)
ATTRIBUTE_ASSIGN_OP_DEF(*=)
ATTRIBUTE_ASSIGN_OP_DEF(/=)

ATTRIBUTE_ASSIGN_OP_NUMERIC_DEF(+=, int)
ATTRIBUTE_ASSIGN_OP_NUMERIC_DEF(-=, int)
ATTRIBUTE_ASSIGN_OP_NUMERIC_DEF(*=, int)
ATTRIBUTE_ASSIGN_OP_NUMERIC_DEF(/=, int)

ATTRIBUTE_COMPARATOR_NUMERIC_DEF(==, int)
ATTRIBUTE_COMPARATOR_NUMERIC_DEF(<, int)
ATTRIBUTE_COMPARATOR_NUMERIC_DEF(>, int)
ATTRIBUTE_COMPARATOR_NUMERIC_DEF(<=, int)

#undef ATTRIBUTE_BIN_OP_DEF
#undef ATTRIBUTE_BIN_OP_NUMERIC_DEF
#undef ATTRIBUTE_ASSIGN_OP_DEF
#undef ATTRIBUTE_ASSIGN_OP_NUMERIC_DEF
#undef ATTRIBUTE_COMPARATOR_NUMERIC_DEF

}  // namespace core
}  // namespace mengde
