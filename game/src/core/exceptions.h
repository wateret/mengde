#ifndef MENGDE_CORE_EXCEPTIONS_H_
#define MENGDE_CORE_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace mengde {
namespace core {

class CoreException : public std::exception {
 public:
  CoreException() : std::exception() {}
};

class DataFormatException : public CoreException {
 public:
  DataFormatException(const std::string& message) : CoreException(), message_(message) {}
  virtual const char* what() const throw() { return message_.c_str(); }

 private:
  std::string message_;
};

class ConfigLoadException : public CoreException {
 public:
  ConfigLoadException(const std::string& message) : CoreException(), message_(message) {}
  virtual const char* what() const throw() { return message_.c_str(); }

 private:
  std::string message_;
};

}  // namespace core
}  // namespace mengde

#endif  // MENGDE_CORE_EXCEPTIONS_H_
