#ifndef __COMMON_EXCEPTIONS_H__
#define __COMMON_EXCEPTIONS_H__

#include <stdexcept>

#define DECLARE_RUNTIME_EXCEPTION(class_name_) \
  class class_name_##Exception \
    : public std::runtime_error \
  { \
  public: \
    class_name_##Exception(std::string const &msg, int code = 0) \
      : std::runtime_error(msg) \
      , Code(code) \
    { \
    } \
    virtual int GetCode() const \
    { \
      return Code; \
    } \
  private: \
    int Code; \
  };

#endif  // !__COMMON_EXCEPTIONS_H__
