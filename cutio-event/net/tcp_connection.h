/**
 * Cutio-Event - A lightweight C++ network library for Linux.
 * Copyright (c) 2024, Niz(mn).  All rights reserved.
 * https://furzoom.com
 * https://github.com/deiio/cutio-event
 */

#ifndef CUTIO_EVENT_NET_TCP_CONNECTION_H_
#define CUTIO_EVENT_NET_TCP_CONNECTION_H_

#include <memory>
#include <utility>

#include <cutio-event/base/noncopyable.h>
#include <cutio-event/base/types.h>
#include <cutio-event/net/callbacks.h>
#include <cutio-event/net/channel_buffer.h>
#include <cutio-event/net/inet_address.h>

namespace cutio {
namespace event {

class Channel;
class EventLoop;
class Socket;

/**
 * TCP connection, for both client and server usage.
 *
 * This is an interface class, so don't expose too much details.
 */
class TcpConnection : public std::enable_shared_from_this<TcpConnection>,
                      noncopyable {
 public:
  /**
   * Constructs a TcpConnection with a connected sockfd.
   */
  TcpConnection(EventLoop* loop, string name, int sockfd,
                const InetAddress& local_addr, const InetAddress& peer_addr);
  ~TcpConnection();

  EventLoop* GetLoop() const { return loop_; }
  const string& Name() const { return name_; }
  const InetAddress& LocalAddr() const { return local_addr_; }
  const InetAddress& PeerAddr() const { return peer_addr_; }
  bool Connected() const { return state_ == kConnected; };
  bool Connecting() const { return state_ == kConnecting; }

  void Shutdown();

  void SetConnectionCallback(ConnectionCallback cb) { connection_cb_ = std::move(cb); }
  void SetMessageCallback(MessageCallback cb) { message_cb_ = std::move(cb); }
  void SetCloseCallback(ConnectionCallback cb) { close_cb_ = std::move(cb); }

  // Called when TcpServer accepts a new connection
  void ConnectEstablished();
  void ConnectDestroyed();

 private:
  void HandleRead();
  void HandleWrite();
  void HandleClose();
  void HandleError();

 private:
  enum States { kDisconnected, kConnecting, kConnected };

  EventLoop* loop_;
  string name_;
  States state_;
  // We don't expose those class to client.
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Channel> channel_;
  InetAddress local_addr_;
  InetAddress peer_addr_;
  ConnectionCallback connection_cb_;
  MessageCallback message_cb_;
  ConnectionCallback close_cb_;
  ChannelBuffer input_buffer_;
  ChannelBuffer output_buffer_;
};

}  // namespace event
}  // namespace cutio

#endif  // CUTIO_EVENT_NET_TCP_CONNECTION_H_
