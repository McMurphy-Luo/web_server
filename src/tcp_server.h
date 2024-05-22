#ifndef WEB_SERVER_TCP_SERVER_H_
#define WEB_SERVER_TCP_SERVER_H_

#pragma once

#include "web_server/web_server.h"
#include "thread_for_io.h"

struct uv_tcp_s;
typedef uv_tcp_s uv_tcp_t;
struct uv_stream_s;
typedef uv_stream_s uv_stream_t;

class TcpServer;

class TcpServerSink {
public:
  virtual ~TcpServerSink() = default;
  virtual void OnConnection(TcpServer* server, int status) = 0;
};

class TcpServer
  : public RefCounter<ThreadUnsafeCounter>
{
public:
  static TcpServer* CreateTcpServer(ThreadForIO* thread);

protected:
  explicit TcpServer(ThreadForIO* thread);
  TcpServer(const TcpServer&) = delete;
  TcpServer(TcpServer&&) = delete;
  TcpServer& operator=(const TcpServer&) = delete;
  TcpServer& operator=(TcpServer&&) = delete;
  virtual ~TcpServer() override;

public:
  void SetSink(TcpServerSink* sink);
  ThreadForIO* Thread();
  uv_tcp_t* Handle();
  void Listen(uint32_t port);
  void Stop();

private:
  static void OnConnection(uv_stream_t* server, int status);
  static void OnTcpClose(uv_handle_t* handle);

private:
  RefCounterPtr<ThreadForIO> thread_;
  uv_tcp_t* tcp_ = nullptr;
  TcpServerSink* sink_ = nullptr;
};

#endif // WEB_SERVER_TCP_SERVER_H_
