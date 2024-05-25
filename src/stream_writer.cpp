#include "stream_writer.h"
#include "tcp_connection.h"

StreamWriter::StreamWriter() {

}

StreamWriter::~StreamWriter() {
  free(handle_);
}

void StreamWriter::AddBuffer(char* buf, size_t buf_size) {
  uv_buf_t uv_buf;
  uv_buf.base = buf;
  uv_buf.len = buf_size;
  buffers_.push_back(uv_buf);
}

void StreamWriter::SetHandle(uv_write_t* handle) {
  handle_ = handle;
}

uv_buf_t* StreamWriter::Buffer() {
  return buffers_.data();
}

size_t StreamWriter::BufferCount() {
  return buffers_.size();
}

void StreamWriter::SetConnection(TcpConnection* connection) {
  connection_ = connection;
}

TcpConnection* StreamWriter::Connection() {
  return connection_.Get();
}
