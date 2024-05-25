#ifndef WEB_SERVER_HTTP_SERVER_H_
#define WEB_SERVER_HTTP_SERVER_H_

#pragma once

#include "web_server/web_server.h"
#include <vector>
#include "ref_counter.h"
#include "tcp_server.h"

class HttpConnection;

class HttpServer final
  : public TcpServer::Delegate
  , public RefCounter<ThreadUnsafeCounter>
{
public:
  explicit HttpServer(TcpServer* server);

protected:
  HttpServer(const HttpServer&) = delete;
  HttpServer(HttpServer&&) = delete;
  HttpServer& operator=(const HttpServer&) = delete;
  HttpServer& operator=(HttpServer&&) = delete;
  virtual ~HttpServer() override;

public:
  void Stop();

private:
  virtual void OnConnection(TcpServer* server, int status) override;

private:
  RefCounterPtr<TcpServer> server_;
  std::vector<RefCounterPtr<HttpConnection>> connections_;
};

#endif // WEB_SERVER_HTTP_SERVER_H_
