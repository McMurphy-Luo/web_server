#ifndef WEB_SERVER_ASYNC_TASK_H_
#define WEB_SERVER_ASYNC_TASK_H_

#pragma once

#include "web_server/web_server.h"
#include "ref_counter.h"
#include "uv.h"

class ThreadForIO;

class AsyncTask final
  : public RefCounter<ThreadUnsafeCounter>
{
public:
  class Delegate {
  public:
    virtual ~Delegate() = default;
    virtual void Execute() = 0;
  };

public:
  static AsyncTask* CreateAsyncTask(ThreadForIO* thread);

protected:
  explicit AsyncTask(ThreadForIO* thread);
  AsyncTask(const AsyncTask&) = delete;
  AsyncTask(AsyncTask&&) = delete;
  AsyncTask& operator=(const AsyncTask&) = delete;
  AsyncTask& operator=(AsyncTask&&) = delete;
  virtual ~AsyncTask() override;

public:
  void SetDelegate(Delegate* p_sink);
  void Submit();

private:
  void SetHandle(uv_async_t* handle);
  static void OnAsyncExecute(uv_async_t* handle);
  static void OnHandleClose(uv_handle_t* handle);

private:
  Delegate* delegate_ = nullptr;
  uv_async_t* handle_ = nullptr;
  RefCounterPtr<ThreadForIO> thread_;
};

#endif // WEB_SERVER_ASYNC_TASK_H_
