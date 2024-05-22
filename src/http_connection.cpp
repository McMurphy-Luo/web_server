#include "http_connection.h"
#include "spdlog/spdlog.h"

HttpConnection::HttpConnection(TcpConnection* conn)
  : conn_(conn)
{

}

void HttpConnection::Close() {
  conn_->Close();
}

void HttpConnection::ResetClose() {
  conn_->ResetClose();
}

void HttpConnection::OnRead(ssize_t nread, char* buffer) {
  std::string readed(buffer, nread);
  SPDLOG_INFO("readed {}", readed);
}
