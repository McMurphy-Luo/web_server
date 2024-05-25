#include "async_task.h"
#include "thread_for_io.h"
#include "uv.h"
#include "spdlog/spdlog.h"

using NAMESPACE::AsyncTask;

AsyncTask* AsyncTask::CreateAsyncTask(ThreadForIO* thread) {
  return new (std::nothrow) AsyncTask(thread);
}

AsyncTask::AsyncTask(ThreadForIO* thread)
  : thread_(thread)
{

}

AsyncTask::~AsyncTask() {

}

void AsyncTask::SetDelegate(Delegate* p_sink) {
  delegate_ = p_sink;
}

void AsyncTask::Submit() {
  if (handle_ == nullptr) {
    handle_ = (uv_async_t*)malloc(sizeof(uv_async_t));
  }
  int result = uv_async_init(thread_->Handle(), handle_, OnAsyncExecute);
  uv_handle_set_data((uv_handle_t*)handle_, this);
  result = uv_async_send(handle_);
  SPDLOG_INFO("result {}", result);
}

void AsyncTask::SetHandle(uv_async_t* handle) {
  handle_ = handle;
}

void AsyncTask::OnAsyncExecute(uv_async_t* handle) {
  AsyncTask* p_this = (AsyncTask*)uv_handle_get_data((uv_handle_t*)handle);
  if (p_this->delegate_) {
    p_this->delegate_->Execute();
  }
  uv_close((uv_handle_t*)handle, OnHandleClose);
}

void AsyncTask::OnHandleClose(uv_handle_t* handle) {
  AsyncTask* p_this = (AsyncTask*)uv_handle_get_data((uv_handle_t*)handle);
  p_this->handle_ = nullptr;
  free(handle);
}
