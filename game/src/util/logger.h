#ifndef LOGGER_H_
#define LOGGER_H_

class Logger {
 public:
  enum LogLevel {
    kLogFatal,
    kLogError,
    kLogWarning,
    kLogDebug,
    kLogInfo,
    kLogAll
  };

 public:
  static Logger* GetInstance();
  const char* LogLevelToString(LogLevel);

 public:
  void SetLevel(LogLevel l) { level_ = l; }
  const char* LogLevelToColor(LogLevel level);
  void Log(LogLevel, const char*, int, const char*, const char*, ...);
  void Log(LogLevel, const char*, ...);

 private:
  Logger();

 private:
  LogLevel level_;
  const char* color_red_;
  const char* color_green_;
  const char* color_yellow_;
  const char* color_blue_;
  const char* color_magenta_;
  const char* color_cyan_;
  const char* color_reset_;
};

#endif // LOGGER_H_
