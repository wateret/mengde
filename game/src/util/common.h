#ifndef COMMON_H_
#define COMMON_H_

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

// Remove this after changing all NULL to nullptr
#ifndef NULL
#define NULL (0)
#endif

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

// TODO move this function somewhere else and remove common.cpp
int GenRandom(int);

#define UNUSED(x) ((void) (x))

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

// Log
#include "logger.h"

#define LOG_INFO(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogInfo, __VA_ARGS__)
#define LOG_DEBUG(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogDebug, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_WARNING(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogWarning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogError, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_FATAL(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogFatal, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#endif // COMMON_H_
