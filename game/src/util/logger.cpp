#include "logger.h"
#include <stdio.h>
#include <stdarg.h>

#define TERM_USE_ANSI_COLOR

Logger::Logger()
    : level_(kLogAll),
      color_reset_(""),
      color_red_(""),
      color_green_(""),
      color_yellow_(""),
      color_blue_(""),
      color_magenta_(""),
      color_cyan_("") {
#ifdef TERM_USE_ANSI_COLOR
  color_reset_   = "\033[0m";
  color_red_     = "\033[1;31m";
  color_green_   = "\033[1;32m";
  color_yellow_  = "\033[1;33m";
  color_blue_    = "\033[1;34m";
  color_magenta_ = "\033[1;35m";
  color_cyan_    = "\033[1;36m";
#endif
}

Logger* Logger::GetInstance() {
  static Logger logger;
  return &logger;
}

void Logger::Log(LogLevel level,
                 const char* module,
                 const char* file,
                 int         line,
                 const char* func,
                 const char* msg,
                 ...) {
  if (level > level_) return;
  va_list args;
  va_start(args, msg);
  fprintf(stderr, "%s[%s]%s%s (%s:%d) ", LogLevelToColor(level), LogLevelToString(level), color_reset_, module, func, line);
  vfprintf(stderr, msg, args);
  fprintf(stderr, "\n");
  va_end(args);
}

void Logger::Log(LogLevel level, const char* module, const char* msg, ...) {
  if (level > level_) return;
  va_list args;
  va_start(args, msg);
  fprintf(stderr, "%s[%s]%s%s ", LogLevelToColor(level), LogLevelToString(level), color_reset_, module);
  vfprintf(stderr, msg, args);
  fprintf(stderr, "\n");
  va_end(args);
}

const char* Logger::LogLevelToColor(LogLevel level) {
  if (level == kLogInfo) return color_green_;
  if (level == kLogDebug) return color_blue_;
  if (level == kLogWarning) return color_yellow_;
  if (level == kLogError) return color_red_;
  if (level == kLogFatal) return color_red_;
  return "";
}

const char* Logger::LogLevelToString(LogLevel level) {
  if (level == kLogInfo) return "INFO";
  if (level == kLogDebug) return "DEBUG";
  if (level == kLogWarning) return "WARNING";
  if (level == kLogError) return "ERROR";
  if (level == kLogFatal) return "FATAL";
  return "";
}

