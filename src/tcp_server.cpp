#include "tcp_server.h"
#include "uv.h"

TcpServer* TcpServer::CreateTcpServer(ThreadForIO* thread) {
  if (!thread) {
    return nullptr;
  }
  return new TcpServer(thread);
}

TcpServer::TcpServer(ThreadForIO* thread)
  : thread_(thread)
{

}

TcpServer::~TcpServer() {

}

ThreadForIO* TcpServer::Thread() {
  return thread_.Get();
}

void TcpServer::Listen(uint32_t port) {
  tcp_ = (uv_tcp_t*)malloc(sizeof(tcp_));
  uv_tcp_init(thread_->Loop(), tcp_);
  sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_port = 8080;
  inet_pton(AF_INET, "0.0.0.0", &address.sin_addr);
  uv_tcp_bind(tcp_, (sockaddr*)(&address), 0);
  uv_listen((uv_stream_t*)tcp_, SOMAXCONN, TcpServer::OnConnection);
}

void TcpServer::Stop() {

}

void TcpServer::OnConnection(uv_stream_t* server, int status) {

}