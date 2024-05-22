#include "tcp_connection.h"
#include "tcp_server.h"
#include "uv.h"
#include "spdlog/spdlog.h"

static void AllocateBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  buf->base = (char*)malloc(suggested_size);
  buf->len = suggested_size;
}

TcpConnection* TcpConnection::AcceptTcpConnection(TcpServer* server) {
  if (!server) {
    return nullptr;
  }
  uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
  uv_tcp_init(server->Thread()->Handle(), client);
  uv_stream_t* server_handle = (uv_stream_t*)server->Handle();
  int result = uv_accept(server_handle, (uv_stream_t*)client);
  SPDLOG_INFO("result {}", result);
  return new (std::nothrow) TcpConnection(client);
}

TcpConnection::TcpConnection(uv_tcp_t* connection_handle)
  : handle_(connection_handle)
{

}

void TcpConnection::ReadStart() {
  uv_read_start((uv_stream_t*)handle_, AllocateBuffer, OnRead);
}

void TcpConnection::OnRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  SPDLOG_INFO("nread {}", nread);
  if (nread > 0) {
    std::string readed(buf->base, nread);
    SPDLOG_INFO("readed {}", readed);
  } else {
    char buf[256];
    memset(buf, 0, 256);
    uv_strerror_r(nread, buf, 256);
    std::string err(buf);
    SPDLOG_INFO("err {}", err);
  }
  if (buf->base != NULL) {
    free(buf->base);
  }
}
