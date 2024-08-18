/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_BASE_LOGGER_H_
#define CUTIO_EVENT_BASE_LOGGER_H_

#include <memory>
#include <ostream>

namespace cutio {
namespace event {

class LoggerImpl;

class Logger {
 public:
  enum LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS,
  };

  Logger(const char* file, int line);
  Logger(const char* file, int line, LogLevel level);
  Logger(const char* file, int line, LogLevel level, const char* func);
  Logger(const char* file, int line, bool to_abort);
  ~Logger();

  std::ostream& stream();

 private:
  std::unique_ptr<LoggerImpl> impl_;
};

#define LOG_TRACE Logger(__FILE__, __LINE__, Logger::TRACE, __func__).stream()
#define LOG_DEBUG Logger(__FILE__, __LINE__, Logger::DEBUG, __func__).stream()
#define LOG_INFO  Logger(__FILE__, __LINE__).stream()
#define LOG_WARN  Logger(__FILE__, __LINE__, Logger::WARN).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::FATAL).stream()
#define LOG_SYSERR Logger(__FILE__, __LINE__, false).stream()
#define LOG_SYSFATAL Logger(__FILE__, __LINE__, true).stream()

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_BASE_LOGGER_H_
