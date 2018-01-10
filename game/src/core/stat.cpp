#include "stat.h"

namespace mengde {
namespace core {

#define ATTRIBUTE_BIN_OP_DEF(OP) \
  Attribute operator OP (const Attribute& lhs, const Attribute& rhs) { \
    return {lhs.atk OP rhs.atk, \
            lhs.def OP rhs.def, \
            lhs.dex OP rhs.dex, \
            lhs.itl OP rhs.itl, \
            lhs.mor OP rhs.mor}; \
  }

#define ATTRIBUTE_BIN_OP_NUMERIC_DEF(OP, TYPE) \
  Attribute operator OP (const Attribute& lhs, TYPE rhs) { \
    return {lhs.atk OP rhs, \
            lhs.def OP rhs, \
            lhs.dex OP rhs, \
            lhs.itl OP rhs, \
            lhs.mor OP rhs}; \
  } \
  \
  Attribute operator OP (TYPE lhs, const Attribute& rhs) { \
    return {lhs OP rhs.atk, \
            lhs OP rhs.def, \
            lhs OP rhs.dex, \
            lhs OP rhs.itl, \
            lhs OP rhs.mor}; \
  }

#define ATTRIBUTE_ASSIGN_OP_DEF(OP) \
  Attribute& operator OP (Attribute& lhs, const Attribute& rhs) { \
    lhs.atk OP rhs.atk; \
    lhs.def OP rhs.def; \
    lhs.dex OP rhs.dex; \
    lhs.itl OP rhs.itl; \
    lhs.mor OP rhs.mor; \
    return lhs; \
  }

#define ATTRIBUTE_ASSIGN_OP_NUMERIC_DEF(OP, TYPE) \
  Attribute& operator OP (Attribute& lhs, TYPE rhs) { \
    lhs.atk OP rhs; \
    lhs.def OP rhs; \
    lhs.dex OP rhs; \
    lhs.itl OP rhs; \
    lhs.mor OP rhs; \
    return lhs; \
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

#undef ATTRIBUTE_BIN_OP_DEF
#undef ATTRIBUTE_BIN_OP_NUMERIC_DEF
#undef ATTRIBUTE_ASSIGN_OP_DEF
#undef ATTRIBUTE_ASSIGN_OP_NUMERIC_DEF

} // namespace core
} // namespace mengde
