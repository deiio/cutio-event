// Copyright (c) 2024 Furzoom.com. All rights reserved.
// Author: mn@furzoom.com
//
// project cutio-event.
//

#include <cutio-event/base/logger.h>

#include <string.h>
#include <unistd.h>

#include <sstream>

#include <cutio-event/base/thread.h>
#include <cutio-event/base/timestamp.h>

namespace cutio {
namespace event {

class LoggerImpl {
 public:
  typedef Logger::LogLevel LogLevel;

  LoggerImpl(LogLevel level, int old_errno, const char* file, int line);

  Timestamp time_;
  std::ostringstream stream_;
  LogLevel level_;
  const char* fullname_;
  int line_;
  const char* basename_;
  const char* function_;

  static const char* LogLevelName[];
};

const char* LoggerImpl::LogLevelName[Logger::NUM_LOG_LEVELS] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL",
};

LoggerImpl::LoggerImpl(cutio::event::LoggerImpl::LogLevel level, int old_errno, const char* file, int line)
  : time_(Timestamp::Now()),
    stream_(),
    level_(level),
    fullname_(file),
    line_(line),
    basename_(nullptr),
    function_(nullptr) {
  const char* path_sep_pos = strrchr(fullname_, '/');
  basename_ = (path_sep_pos != nullptr) ? path_sep_pos + 1 : fullname_;
  char message_head[512];
  snprintf(message_head, sizeof(message_head), "%s %5d %s:%d %s ",
           time_.ToFormattedString().c_str(), CurrentThread::tid(),
           basename_, line, LogLevelName[level]);
  stream_ << message_head;
  if (old_errno != 0) {
    stream_ << strerror(old_errno);
  }
}

Logger::Logger(const char* file, int line)
  : impl_(new LoggerImpl(INFO, 0, file, line)) {}

Logger::Logger(const char* file, int line, Logger::LogLevel level)
  : impl_(new LoggerImpl(level, 0, file, line)) {}

Logger::Logger(const char* file, int line, Logger::LogLevel level, const char* func)
  : impl_(new LoggerImpl(level, 0, file, line)) {
  impl_->stream_ << func << "() ";
}

Logger::Logger(const char* file, int line, bool to_abort)
  : impl_(new LoggerImpl(to_abort ? FATAL : ERROR, errno, file, line)) {}

Logger::~Logger() {
  impl_->stream_ << '\n';
  std::string buf(impl_->stream_.str());
  ::write(STDOUT_FILENO, buf.data(), buf.size());
  if (impl_->level_ == FATAL) {
    abort();
  }
}

std::ostream& Logger::stream() {
  return impl_->stream_;
}

}  // namespace event
}  // namespace cutio
