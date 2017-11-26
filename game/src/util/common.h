#ifndef COMMON_H_
#define COMMON_H_

// Remove this after changing all NULL to nullptr
#ifndef NULL
#define NULL (0)
#endif

#define UNUSED(x) ((void) (x))

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
using std::string;
using std::stack;
using std::unique_ptr;
using std::shared_ptr;
using std::vector;

#include "vec2d.h"
#include "direction.h"
#include "misc_helpers.h"
#include "assert_helper.h"
#include "logger.h"

#endif // COMMON_H_
