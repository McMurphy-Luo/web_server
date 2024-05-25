#include "http_connection.h"
#include "spdlog/spdlog.h"
#include "stream_writer.h"

constexpr char kResponseOK[] =
R"(HTTP/1.1 200 OK
Content-Type: text/plain
Connection: close

Hello World)";

HttpConnection::HttpConnection(TcpConnection* conn)
  : conn_(conn)
{

}

HttpConnection::~HttpConnection() {

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
  StreamWriter* writer = new StreamWriter();
  SPDLOG_INFO("write {}", kResponseOK);
  assert(kResponseOK[ARRAYSIZE(kResponseOK) - 1] == 0);
  writer->AddBuffer((char*)kResponseOK, ARRAYSIZE(kResponseOK) - 1);
  conn_->Write(writer);
}

void HttpConnection::OnWrite(StreamWriter* writter) {
  Close();
}
