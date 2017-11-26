#include <stdio.h>
#include <stdlib.h>

// ASSERT, UNREACHABLE macro
#ifdef DEBUG

#define ASSERT(x) \
  do \
  { \
    if (!(x)) \
    { \
      fprintf(stderr, \
              "\n" \
              "******** ASSERTION \"%s\" FAILED ********\n" \
              "%s:%d In function '%s'\n" \
              "\n", \
              #x, \
               __FILE__, \
               __LINE__, \
               __func__); \
      abort(); \
    } \
  } while (0)

#define UNREACHABLE(msg) \
  do \
  { \
    fprintf(stderr, \
            "\n" \
            "******** UNREACHABLE CODE \"%s\" ********\n" \
            "%s:%d In function '%s'\n" \
            "\n", \
            msg, \
             __FILE__, \
             __LINE__, \
             __func__); \
    abort(); \
  } while (0)

#else // DEBUG

#define ASSERT(x) ((void) 0)
#define UNREACHABLE(x) ((void) 0)

#endif // !DEBUG

