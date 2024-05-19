#ifndef WEB_SERVER_THREAD_IO_H_
#define WEB_SERVER_THREAD_IO_H_

#pragma once

#include "web_server/web_server.h"
#include <memory>
#include <thread>
#include <atomic>

struct uv_loop_s;
typedef struct uv_loop_s uv_loop_t;

class ThreadIO {
public:
  ThreadIO();

  ThreadIO(const ThreadIO&) = delete;
  ThreadIO& operator=(const ThreadIO&) = delete;
  ThreadIO(ThreadIO&&) = delete;
  ThreadIO& operator=(ThreadIO&&) = delete;

  void Start();
  void Stop();

private:
  void ThreadProc();

private:
  std::unique_ptr<std::thread> thread_;
  uv_loop_t* loop_ = nullptr;
  std::atomic<bool> running_;
  std::atomic<bool> started_;
};

#endif // WEB_SERVER_THREAD_IO_H_
