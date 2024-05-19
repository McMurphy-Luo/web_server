#include "tcp_server.h"


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

}

void TcpServer::Stop() {

}
