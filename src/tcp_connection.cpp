#include "tcp_connection.h"
#include "tcp_server.h"
#include "uv.h"
#include "spdlog/spdlog.h"
#include "stream_writer.h"

static void AllocateBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
  buf->base = (char*)malloc(suggested_size);
  buf->len = static_cast<ULONG>(suggested_size);
}

TcpConnection* TcpConnection::AcceptTcpConnection(TcpServer* server) {
  if (!server) {
    return nullptr;
  }
  TcpConnection* conn = new (std::nothrow) TcpConnection();
  if (!conn) {
    return nullptr;
  }
  uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
  uv_tcp_init(server->Thread()->Handle(), client);
  uv_handle_set_data((uv_handle_t*)client, conn);
  uv_stream_t* server_handle = (uv_stream_t*)server->Handle();
  int result = uv_accept(server_handle, (uv_stream_t*)client);
  SPDLOG_INFO("result {}", result);
  conn->SetHandle(client);
  return conn;
}

TcpConnection::TcpConnection()
{

}

void TcpConnection::SetDelegate(Delegate* sink) {
  delegate_ = sink;
}

void TcpConnection::ReadStart() {
  if (handle_) {
    uv_read_start((uv_stream_t*)handle_, AllocateBuffer, OnRead);
  }
}

void TcpConnection::ReadStop() {
  if (handle_) {
    uv_read_stop((uv_stream_t*)handle_);
  }
}

void TcpConnection::Close() {
  if (handle_) {
    uv_close((uv_handle_t*)handle_, OnConnectionClose);
  }
}

void TcpConnection::ResetClose() {
  if (handle_) {
    uv_tcp_close_reset(handle_, OnConnectionClose);
  }
}

void TcpConnection::Write(StreamWriter* writer) {
  uv_write_t* handle = (uv_write_t*)malloc(sizeof(uv_write_t));
  writer->Increment();
  writer->SetHandle(handle);
  writer->SetConnection(this);
  uv_req_set_data((uv_req_t*)handle, writer);
  uv_write(handle, (uv_stream_t*)handle_, writer->Buffer(), writer->BufferCount(), OnWrite);
}

void TcpConnection::OnRead(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
  SPDLOG_INFO("nread {}", nread);
  if (nread > 0) {
    TcpConnection* conn = (TcpConnection*)uv_handle_get_data((uv_handle_t*)stream);
    if (conn->delegate_) {
      conn->delegate_->OnRead(nread, buf->base);
    }
  } else {
    char buf[256];
    memset(buf, 0, 256);
    uv_strerror_r(static_cast<int>(nread), buf, 256);
    std::string err(buf);
    SPDLOG_INFO("err {}", err);
  }
  if (buf->base != NULL) {
    free(buf->base);
  }
}

void TcpConnection::OnWrite(uv_write_t* req, int status) {
  StreamWriter* writer = (StreamWriter*)uv_req_get_data((uv_req_t*)req);
  TcpConnection* conn = writer->Connection();
  if (conn->delegate_) {
    conn->delegate_->OnWrite(writer);
  }
  writer->SetConnection(nullptr);
  writer->Decrement();
}

void TcpConnection::OnConnectionClose(uv_handle_t* handle) {
  TcpConnection* server = (TcpConnection*)uv_handle_get_data(handle);
  server->handle_ = nullptr;
  free(handle);
}

void TcpConnection::OnShutDown(uv_shutdown_t* req, int status) {
  uv_close((uv_handle_t*)req->handle, OnConnectionClose);
  free(req);
}

void TcpConnection::SetHandle(uv_tcp_t* handle) {
  handle_ = handle;
}
