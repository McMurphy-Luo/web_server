#ifndef WEB_SERVER_THREAD_FOR_IO_H_
#define WEB_SERVER_THREAD_FOR_IO_H_

#pragma once

#include "web_server/web_server.h"
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include "ref_counter.h"
#include "uv.h"

class AsyncTask;

class ThreadForIO
  : public RefCounter<ThreadSafeCounter>
{
public:
  static ThreadForIO* CreateThread();

protected:
  explicit ThreadForIO();
  ThreadForIO(const ThreadForIO&) = delete;
  ThreadForIO& operator=(const ThreadForIO&) = delete;
  ThreadForIO(ThreadForIO&&) = delete;
  ThreadForIO& operator=(ThreadForIO&&) = delete;
  virtual ~ThreadForIO() override;

public:
  void Start();
  void Stop();
  uv_loop_t* Handle();
  void Wake();

private:
  void ThreadProc();

private:
  std::unique_ptr<std::thread> thread_;
  std::condition_variable sleep_var_;
  std::mutex loop_guard_;
  std::atomic<bool> should_terminate_;
  uv_loop_t* loop_ = nullptr;
};

#endif // WEB_SERVER_THREAD_FOR_IO_H_
