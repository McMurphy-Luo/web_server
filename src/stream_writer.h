#ifndef WEB_SERVER_STREAM_WRITTER_H_
#define WEB_SERVER_STREAM_WRITTER_H_

#pragma once

#include "web_server/web_server.h"
#include "uv.h"
#include "ref_counter.h"
#include <vector>

class TcpConnection;

class StreamWriter final
  : public RefCounter<ThreadUnsafeCounter>
{
  friend class TcpConnection;

public:
  explicit StreamWriter();

protected:
  StreamWriter(const StreamWriter&) = delete;
  StreamWriter(StreamWriter&&) = delete;
  StreamWriter& operator=(const StreamWriter&) = delete;
  StreamWriter& operator=(StreamWriter&&) = delete;
  virtual ~StreamWriter() override;

public:
  void AddBuffer(char* p_buf, size_t buf_size);

private:
  void SetHandle(uv_write_t* handle);
  uv_buf_t* Buffer();
  size_t BufferCount();
  void SetConnection(TcpConnection* connection);
  TcpConnection* Connection();

private:
  std::vector<uv_buf_t> buffers_;
  uv_write_t* handle_ = nullptr;
  RefCounterPtr<TcpConnection> connection_;
};

#endif // WEB_SERVER_STREAM_WRITTER_H_
