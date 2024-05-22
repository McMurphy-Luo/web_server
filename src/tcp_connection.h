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
struct uv_handle_s;
typedef uv_handle_s uv_handle_t;
struct uv_write_s;
typedef uv_write_s uv_write_t;
struct uv_shutdown_s;
typedef uv_shutdown_s uv_shutdown_t;
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
  explicit TcpConnection();
  TcpConnection(const TcpConnection&) = delete;
  TcpConnection(TcpConnection&&) = delete;
  TcpConnection& operator=(const TcpConnection&) = delete;
  TcpConnection& operator=(TcpConnection&&) = delete;

public:
  void SetSink(TcpConnectionSink* sink);
  void ReadStart();
  void ReadStop();
  void Close();
  void ResetClose();

private:
  static void OnRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
  static void OnWrite(uv_write_t* req, int status);
  static void OnConnectionClose(uv_handle_t* handle);
  static void OnShutDown(uv_shutdown_t* req, int status);
  void SetHandle(uv_tcp_t* handle);

private:
  TcpConnectionSink* sink_ = nullptr;
  uv_tcp_t* handle_ = nullptr;
};

#endif // WEB_SERVER_TCP_CONNECTION_H_
