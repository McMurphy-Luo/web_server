#ifndef WEB_SERVER_TCP_CONNECTION_H_
#define WEB_SERVER_TCP_CONNECTION_H_

#pragma once

#include "web_server/web_server.h"
#include "ref_counter.h"
#include "uv.h"

NAMESPACE_BEGIN

class TcpServer;
class StreamWriter;

class TcpConnection
  : public RefCounter<ThreadUnsafeCounter>
{
public:
  class Delegate {
  public:
    virtual ~Delegate() = default;
    virtual void OnRead(ssize_t nread, char* buffer) = 0;
    virtual void OnWrite(StreamWriter* writter) = 0;
  };

public:
  static TcpConnection* AcceptTcpConnection(TcpServer* server);

protected:
  explicit TcpConnection();
  TcpConnection(const TcpConnection&) = delete;
  TcpConnection(TcpConnection&&) = delete;
  TcpConnection& operator=(const TcpConnection&) = delete;
  TcpConnection& operator=(TcpConnection&&) = delete;

public:
  void SetDelegate(Delegate* sink);
  void ReadStart();
  void ReadStop();
  void Close();
  void ResetClose();
  void Write(StreamWriter* writter);

private:
  static void OnRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
  static void OnWrite(uv_write_t* req, int status);
  static void OnConnectionClose(uv_handle_t* handle);
  static void OnShutDown(uv_shutdown_t* req, int status);
  void SetHandle(uv_tcp_t* handle);

private:
  Delegate* delegate_ = nullptr;
  uv_tcp_t* handle_ = nullptr;
};

NAMESPACE_END

#endif // WEB_SERVER_TCP_CONNECTION_H_
