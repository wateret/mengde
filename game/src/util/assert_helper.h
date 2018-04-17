#ifndef UTIL_ASSERT_HELPER_H_
#define UTIL_ASSERT_HELPER_H_

#include <stdio.h>
#include <stdlib.h>

// ASSERT, UNREACHABLE macro
#ifdef DEBUG

#define ASSERT(x)                                           \
  do {                                                      \
    if (!(x)) {                                             \
      fprintf(stderr,                                       \
              "\n"                                          \
              "******** ASSERTION \"%s\" FAILED ********\n" \
              "%s:%d In function '%s'\n"                    \
              "\n",                                         \
              #x, __FILE__, __LINE__, __func__);            \
      abort();                                              \
    }                                                       \
  } while (0)

#define ASSERT_BIN_(A, OP, B)                                                      \
  do {                                                                             \
    if (!((A)OP(B))) {                                                             \
      fprintf(stderr,                                                              \
              "\n"                                                                 \
              "******** ASSERTION \"%s %s %s\" FAILED ********\n"                  \
              "%s:%d In function '%s'\n"                                           \
              "\n"                                                                 \
              "Compare Result : (%d %s %d)\n"                                      \
              "\n",                                                                \
              #A, #OP, #B, __FILE__, __LINE__, __func__, (int)(A), #OP, (int)(B)); \
      abort();                                                                     \
    }                                                                              \
  } while (0)

#define ASSERT_EQ(A, B) ASSERT_BIN_(A, ==, B)

#define ASSERT_LT(A, B) ASSERT_BIN_(A, <, B)

#define ASSERT_LE(A, B) ASSERT_BIN_(A, <=, B)

#define ASSERT_GT(A, B) ASSERT_BIN_(A, >, B)

#define ASSERT_GE(A, B) ASSERT_BIN_(A, >=, B)

#define UNREACHABLE(msg)                                  \
  do {                                                    \
    fprintf(stderr,                                       \
            "\n"                                          \
            "******** UNREACHABLE CODE \"%s\" ********\n" \
            "%s:%d In function '%s'\n"                    \
            "\n",                                         \
            msg, __FILE__, __LINE__, __func__);           \
    abort();                                              \
  } while (0)

#else  // DEBUG

#define ASSERT(x) ((void)sizeof(x))
#define ASSERT_BIN_(A, B) \
  do {                    \
    ((void)sizeof(A));    \
    ((void)sizeof(B));    \
  } while (0)
#define ASSERT_EQ(A, B) ASSERT_BIN_(A, B)
#define ASSERT_LT(A, B) ASSERT_BIN_(A, B)
#define ASSERT_LE(A, B) ASSERT_BIN_(A, B)
#define ASSERT_GT(A, B) ASSERT_BIN_(A, B)
#define ASSERT_GE(A, B) ASSERT_BIN_(A, B)

#define UNREACHABLE(x) ((void)0)

#endif  // !DEBUG

#endif // UTIL_ASSERT_HELPER_H_
