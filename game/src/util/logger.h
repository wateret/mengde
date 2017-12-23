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
  void Log(LogLevel, const char*, const char*, int, const char*, const char*, ...);
  void Log(LogLevel, const char*, const char*, ...);

 private:
  Logger();

 private:
  LogLevel level_;
  const char* color_reset_;
  const char* color_red_;
  const char* color_green_;
  const char* color_yellow_;
  const char* color_blue_;
  const char* color_magenta_;
  const char* color_cyan_;
};

#ifdef DEBUG

#define LOGM_INFO(m, ...) Logger::GetInstance()->Log(Logger::LogLevel::kLogInfo, "[" #m "]", __VA_ARGS__)
#define LOGM_DEBUG(m, ...) Logger::GetInstance()->Log(Logger::LogLevel::kLogDebug, "[" #m "]", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOGM_WARNING(m, ...) Logger::GetInstance()->Log(Logger::LogLevel::kLogWarning, "[" #m "]", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOGM_ERROR(m, ...) Logger::GetInstance()->Log(Logger::LogLevel::kLogError, "[" #m "]", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOGM_FATAL(m, ...) Logger::GetInstance()->Log(Logger::LogLevel::kLogFatal, "[" #m "]", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#define LOG_INFO(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogInfo, "", __VA_ARGS__)
#define LOG_DEBUG(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogDebug, "", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_WARNING(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogWarning, "", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_ERROR(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogError, "", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define LOG_FATAL(...) Logger::GetInstance()->Log(Logger::LogLevel::kLogFatal, "", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#else // !DEBUG

#define LOGM_INFO(m, ...) ((void) 0)
#define LOGM_DEBUG(m, ...) ((void) 0)
#define LOGM_WARNING(m, ...) ((void) 0)
#define LOGM_ERROR(m, ...) ((void) 0)
#define LOGM_FATAL(m, ...) ((void) 0)

#define LOG_INFO(...) ((void) 0)
#define LOG_DEBUG(...) ((void) 0)
#define LOG_WARNING(...) ((void) 0)
#define LOG_ERROR(...) ((void) 0)
#define LOG_FATAL(...) ((void) 0)

#endif // !DEBUG

#endif // LOGGER_H_
