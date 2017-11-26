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

#define LOG_INFO(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogInfo, __VA_ARGS__)
#define LOG_DEBUG(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogDebug, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_WARNING(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogWarning, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogError, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_FATAL(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogFatal, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#endif // LOGGER_H_
