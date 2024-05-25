#ifndef WEB_SERVER_HTTP_CONNECTION_H_
#define WEB_SERVER_HTTP_CONNECTION_H_

#pragma once

#include "web_server/web_server.h"
#include "tcp_connection.h"

class HttpConnection final
  : public TcpConnection::Delegate
  , public RefCounter<ThreadUnsafeCounter>
{
public:
  explicit HttpConnection(TcpConnection*);

protected:
  HttpConnection(const HttpConnection&) = delete;
  HttpConnection(HttpConnection&&) = delete;
  HttpConnection& operator=(const HttpConnection&) = delete;
  HttpConnection& operator=(HttpConnection&&) = delete;
  virtual ~HttpConnection() override;

public:
  void Close();
  void ResetClose();

private:
  virtual void OnRead(ssize_t nread, char* buffer) override;
  virtual void OnWrite(StreamWriter* writter) override;

private:
  RefCounterPtr<TcpConnection> conn_;
};

#endif // WEB_SERVER_HTTP_CONNECTION_H_
