/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#include <cutio-event/net/channel_buffer.h>

#include <sys/uio.h>

namespace cutio {
namespace event {

char* ChannelBuffer::Retrieve(size_t len) {
  assert(len < ReadableBytes());
  char* p = Begin() + reader_index_;
  reader_index_ += len;
  return p;
}

string ChannelBuffer::RetrieveAsString() {
  string str(Peek(), ReadableBytes());
  reader_index_ = kCheapPrepend;
  writer_index_ = kCheapPrepend;
  return str;
}

void ChannelBuffer::Append(const char* data, size_t len) {
  if (WritableBytes() < len) {
    MakeSpace(len);
  }
  assert(WritableBytes() >= len);
  std::copy(data, data + len, Begin() + writer_index_);
  writer_index_ += len;
}

void ChannelBuffer::Prepend(const char* data, size_t len) {
  assert(len <= PrependableBytes());
  reader_index_ -= len;
  std::copy(data, data + len, Begin() + reader_index_);
}

ssize_t ChannelBuffer::ReadFd(int fd, int* saved_errno) {
  char extra_buf[64 * 1024];
  iovec vec[2];
  auto writable = WritableBytes();
  vec[0].iov_base = Begin() + writer_index_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extra_buf;
  vec[1].iov_len = sizeof(extra_buf);
  auto n = readv(fd, vec, sizeof(vec) / sizeof(vec[0]));
  if (n < 0) {
    *saved_errno = errno;
  } else if (static_cast<size_t>(n) < writable) {
    writer_index_ += static_cast<size_t>(n);
  } else {
    writer_index_ = buffer_.size();
    Append(extra_buf, static_cast<size_t>(n) - writable);
  }
  return n;
}

void ChannelBuffer::MakeSpace(size_t more) {
  if (WritableBytes() + PrependableBytes() < more + kCheapPrepend) {
    buffer_.resize(writer_index_ + more);
  } else {
    // Move readable data to the front, make sure inside buffer.
    auto used = ReadableBytes();
    std::copy(Begin() + reader_index_, Begin() + writer_index_, Begin() + kCheapPrepend);
    reader_index_ = kCheapPrepend;
    writer_index_ = reader_index_ + used;
    assert(used == ReadableBytes());
  }
}

}  // namespace event
}  // namespace cutio
