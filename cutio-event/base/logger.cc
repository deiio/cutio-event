/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 *
 * BSD 3-Clause License
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     - Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     - Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     - Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

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
