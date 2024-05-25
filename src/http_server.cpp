#include "http_server.h"
#include "tcp_connection.h"
#include "http_connection.h"
#include <algorithm>
#include "spdlog/spdlog.h"
#include "uv.h"

static void CloseConnection(RefCounterPtr<HttpConnection> conn) {
  conn->Close();
}

HttpServer::HttpServer(TcpServer* server)
  : server_(server)
{

}

HttpServer::~HttpServer() {
  server_->SetDelegate(nullptr);
}

void HttpServer::Stop() {
  std::for_each(connections_.begin(), connections_.end(), CloseConnection);
  server_->Stop();
}

void HttpServer::OnConnection(TcpServer* server, int status) {
  if (status < 0) {
    char buf[256];
    memset(buf, 0, 256);
    uv_strerror_r(status, buf, 256);
    std::string error(buf);
    SPDLOG_INFO("New connection error {}", error);
    return;
  }
  TcpConnection* conn = TcpConnection::AcceptTcpConnection(server);
  HttpConnection* http_conn = new (std::nothrow) HttpConnection(conn);
  conn->SetDelegate(http_conn);
  conn->ReadStart();
  connections_.push_back(http_conn);
}
