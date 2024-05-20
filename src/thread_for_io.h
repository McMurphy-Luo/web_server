#ifndef WEB_SERVER_THREAD_FOR_IO_H_
#define WEB_SERVER_THREAD_FOR_IO_H_

#pragma once

#include "web_server/web_server.h"
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>
#include "ref_counter.h"

struct uv_loop_s;
struct uv_async_s;
struct uv_handle_s;
typedef struct uv_loop_s uv_loop_t;
typedef struct uv_async_s uv_async_t;
typedef struct uv_handle_s uv_handle_t;

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
  static void TerminateLoop(uv_async_t* handle);
  static void CleanHandle(uv_handle_t* handle);

private:
  std::unique_ptr<std::thread> thread_;
  std::condition_variable sleep_var_;
  std::mutex loop_guard_;
  std::atomic<bool> started_;
  uv_loop_t* loop_ = nullptr;
  uv_async_t* loop_terminator_ = nullptr;
  std::atomic<bool> running_;
};

#endif // WEB_SERVER_THREAD_FOR_IO_H_
