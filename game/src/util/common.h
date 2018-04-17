#ifndef UTIL_COMMON_H_
#define UTIL_COMMON_H_

// Remove this after changing all NULL to nullptr
#ifndef NULL
#define NULL (0)
#endif

#define UNUSED(x) ((void)(x))

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <deque>
#include <functional>
#include <list>
#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <vector>

// more typedefs for commonly used types
using std::deque;
using std::function;
using std::list;
using std::queue;
using std::shared_ptr;
using std::stack;
using std::string;
using std::unique_ptr;
using std::vector;

#include "assert_helper.h"
#include "direction.h"
#include "logger.h"
#include "misc_helpers.h"
#include "vec2d.h"

#endif  // UTIL_COMMON_H_
