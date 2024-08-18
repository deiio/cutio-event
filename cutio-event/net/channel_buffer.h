/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

// This si a public header file, it must only include public header files.

#ifndef CUTIO_EVENT_NET_CHANNEL_BUFFER_H_
#define CUTIO_EVENT_NET_CHANNEL_BUFFER_H_

#include <assert.h>

#include <vector>

#include <cutio-event/base/copyable.h>
#include <cutio-event/base/types.h>

namespace cutio {
namespace event {

/**
 * A buffer class modeled after org.jboss.netty.buffer.ChannelBuffer
 *
 * @code
 * +-------------------+------------------+------------------+
 * | prependable bytes |  readable bytes  |  writable bytes  |
 * |                   |    (CONTENT)     |                  |
 * +-------------------+------------------+------------------+
 * |                   |                  |                  |
 * 0       <=     readerIndex   <=    writerIndex   <=      size
 * @endcode
 */
class ChannelBuffer : public copyable {
 public:
  static const size_t kCheapPrepend = 8;
  static const size_t kInitialSize = 1024;

  ChannelBuffer()
    : buffer_(kCheapPrepend + kInitialSize),
      reader_index_(kCheapPrepend),
      writer_index_(kCheapPrepend) {
    assert(ReadableBytes() == 0);
    assert(WritableBytes() == kInitialSize);
    assert(PrependableBytes() == kCheapPrepend);
  }

  size_t ReadableBytes() const { return writer_index_ - reader_index_; }
  size_t WritableBytes() const { return buffer_.size() - writer_index_; }
  size_t PrependableBytes() const { return reader_index_; }

  const char* Peek() const { return Begin() + reader_index_; }
  char* Retrieve(size_t len);
  string RetrieveAsString();
  void Append(const char* data, size_t len);
  void Prepend(const char* data, size_t len);
  ssize_t ReadFd(int fd, int* saved_errno);

 private:
  char* Begin() { return &*buffer_.begin(); }
  const char* Begin() const { return &*buffer_.begin(); }
  void MakeSpace(size_t more);

 private:
  std::vector<char> buffer_;
  size_t reader_index_;
  size_t writer_index_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_CHANNEL_BUFFER_H_
