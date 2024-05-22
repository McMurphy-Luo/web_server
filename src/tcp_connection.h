#ifndef WEB_SERVER_TCP_CONNECTION_H_
#define WEB_SERVER_TCP_CONNECTION_H_

#pragma once

#include "web_server/web_server.h"
#include "ref_counter.h"

struct uv_tcp_s;
typedef uv_tcp_s uv_tcp_t;
struct uv_stream_s;
typedef uv_stream_s uv_stream_t;
struct uv_buf_t;
typedef intptr_t ssize_t;

class TcpServer;

class TcpConnectionSink {
public:
  virtual ~TcpConnectionSink() = default;
  virtual void OnRead(ssize_t nread, char* buffer) = 0;
};

class TcpConnection
  : public RefCounter<ThreadUnsafeCounter>
{
public:
  static TcpConnection* AcceptTcpConnection(TcpServer* server);

protected:
  explicit TcpConnection(uv_tcp_t* connection_handle);
  TcpConnection(const TcpConnection&) = delete;
  TcpConnection(TcpConnection&&) = delete;
  TcpConnection& operator=(const TcpConnection&) = delete;
  TcpConnection& operator=(TcpConnection&&) = delete;

public:
  void SetSink(TcpConnectionSink* sink);
  void ReadStart();

private:
  static void OnRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);

private:
  uv_tcp_t* handle_ = nullptr;
};

#endif // WEB_SERVER_TCP_CONNECTION_H_
