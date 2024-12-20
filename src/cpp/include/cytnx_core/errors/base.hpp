#ifndef CYTNX_CORE_BASE_ERROR_H_
#define CYTNX_CORE_BASE_ERROR_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <execinfo.h>
#include <iostream>
#include <stdarg.h>
#include <stdexcept>

#ifdef _MSC_VER
  #define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define cytnx_error_msg(is_true, format, ...)                                               \
  {                                                                                         \
    if (is_true)                                                                            \
      error_msg(__PRETTY_FUNCTION__, __FILE__, __LINE__, (is_true), (format), __VA_ARGS__); \
  }
static inline void error_msg(char const *const func, const char *const file, int const line,
                             bool is_true, char const *format, ...) {
  // try {
  if (is_true) {
    va_list args;
    char output_str[1024];
    char msg[512];
    va_start(args, format);
    vsprintf(msg, format, args);
    sprintf(output_str, "\n# Cytnx error occur at %s\n# error: %s\n# file : %s (%d)", func, msg,
            file, line);
    va_end(args);
    // std::cerr << output_str << std::endl;
    std::cerr << output_str << std::endl;
    std::cerr << "Stack trace:" << std::endl;
    void *array[10];
    size_t size;
    size = backtrace(array, 10);
    char **strings = backtrace_symbols(array, size);
    for (size_t i = 0; i < size; i++) {
      std::cerr << strings[i] << std::endl;
    }
    free(strings);
    throw std::logic_error(output_str);
  }
  // } catch (const char *output_msg) {
  //   std::cerr << output_msg << std::endl;
  // }
}
#define cytnx_warning_msg(is_true, format, ...)                                               \
  {                                                                                           \
    if (is_true)                                                                              \
      warning_msg(__PRETTY_FUNCTION__, __FILE__, __LINE__, (is_true), (format), __VA_ARGS__); \
  }
static inline void warning_msg(char const *const func, const char *const file, int const line,
                               bool is_true, char const *format, ...) {
  if (is_true) {
    va_list args;
    char output_str[1024];
    char msg[512];
    va_start(args, format);
    vsprintf(msg, format, args);
    sprintf(output_str, "\n# Cytnx warning occur at %s\n# warning: %s\n# file : %s (%d)", func, msg,
            file, line);
    va_end(args);
    std::cerr << output_str << std::endl;
  }
}

#endif  // CYTNX_CORE_BASE_ERROR_H_
