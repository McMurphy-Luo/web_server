#include "tcp_server.h"
#include "uv.h"
#include "spdlog/spdlog.h"

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

void TcpServer::SetSink(TcpServerSink* sink) {
  sink_ = sink;
}

ThreadForIO* TcpServer::Thread() {
  return thread_.Get();
}

uv_tcp_t* TcpServer::Handle() {
  return tcp_;
}

void TcpServer::Listen(uint32_t port) {
  tcp_ = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
  uv_tcp_init(thread_->Handle(), tcp_);
  uv_handle_set_data((uv_handle_t*)tcp_, this);
  sockaddr_in address;
  uv_ip4_addr("0.0.0.0", port, &address);
  int result = uv_tcp_bind(tcp_, (sockaddr*)(&address), 0);
  result = uv_listen((uv_stream_t*)tcp_, SOMAXCONN, TcpServer::OnConnection);
}

void TcpServer::Stop() {
  uv_tcp_close_reset(tcp_, OnTcpClose);
}

void TcpServer::OnConnection(uv_stream_t* server, int status) {
  SPDLOG_INFO("status {}", status);
  TcpServer* the_server = (TcpServer*)uv_handle_get_data((uv_handle_t*)server);
  if (the_server->sink_) {
    the_server->sink_->OnConnection(the_server, status);
  }
}

void TcpServer::OnTcpClose(uv_handle_t* handle) {
  TcpServer* server = (TcpServer*)uv_handle_get_data(handle);
  server->tcp_ = nullptr;
  free(handle);
}
