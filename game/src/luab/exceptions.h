#ifndef LUAB_EXCEPTIONS_H_
#define LUAB_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

#include "ref.h"

namespace luab {

class LuaException : public std::exception {
 public:
  LuaException() : std::exception() {}
};

class UndeclaredVariableException : public LuaException {
 public:
  UndeclaredVariableException(const std::string& var_expr) : LuaException(), var_expr_(var_expr) {}
  virtual const char* what() const throw() { return (var_expr_ + " is undeclared.").c_str(); }

 private:
  std::string var_expr_;
};

class WrongTypeException : public LuaException {
 public:
  WrongTypeException(const std::string& type_expected, const std::string& type_actual)
      : LuaException(), type_expected_(type_expected), type_actual_(type_actual) {}
  virtual const char* what() const throw() {
    return ("Wrong type - (Expected: " + type_expected_ + ", Actual: " + type_actual_ + ")").c_str();
  }

 private:
  std::string type_expected_;
  std::string type_actual_;
};

class ScriptRuntimeException : public LuaException {
 public:
  ScriptRuntimeException() = default;
  ScriptRuntimeException(const std::string& message) : LuaException(), message_(message) {}
  virtual const char* what() const throw() { return message_.c_str(); }

 protected:
  std::string message_;
};

class UncallableException : public LuaException {
 public:
  UncallableException(const Ref& ref) : LuaException{}, ref_{ref} {}
  virtual const char* what() const throw() {
    static const std::string actual_message =
        "Tried to call a value which is not a function (reference : " + std::to_string(ref_.value()) + ")";
    return actual_message.c_str();
  }

 private:
  Ref ref_;
};

class ScriptSyntaxException : public ScriptRuntimeException {
 public:
  ScriptSyntaxException(const std::string& message) : ScriptRuntimeException{message} {}
  virtual const char* what() const throw() {
    static const std::string actual_message = "lua script syntax error - " + message_;
    return actual_message.c_str();
  }
};

}  // namespace luab

#endif  // LUAB_EXCEPTIONS_H_
