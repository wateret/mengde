#ifndef CORE_DATA_LOAD_EXCEPTION_H_
#define CORE_DATA_LOAD_EXCEPTION_H_

#include <stdexcept>
#include <string>

namespace mengde {
namespace core {

class DataLoadException : public std::exception {
 public:
  DataLoadException() : std::exception() {}
};

class DataFormatException : public DataLoadException {
 public:
  DataFormatException(const std::string& message) : DataLoadException(), message_(message) {}
  virtual const char* what() const throw() { return message_.c_str(); }

 private:
  std::string message_;
};

} // namespace mengde
} // namespace core

#endif // CORE_DATA_LOAD_EXCEPTION_H_
