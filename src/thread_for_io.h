#ifndef WEB_SERVER_THREAD_FOR_IO_H_
#define WEB_SERVER_THREAD_FOR_IO_H_

#pragma once

#include "web_server/web_server.h"
#include <memory>
#include <thread>
#include <atomic>
#include "ref_counter.h"

struct uv_loop_s;
typedef struct uv_loop_s uv_loop_t;

class ThreadForIO : public RefCounter<ThreadSafeCounter>
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

private:
  void ThreadProc();

private:
  std::unique_ptr<std::thread> thread_;
  std::condition_variable sleep_var_;
  std::mutex mutex_of_sleep_;
  uv_loop_t* loop_ = nullptr;
  std::atomic<bool> running_;
  std::atomic<bool> started_;
};

#endif // WEB_SERVER_THREAD_FOR_IO_H_
