#include "thread_for_io.h"
#include <sstream>
#include "uv.h"
#include "spdlog/spdlog.h"

ThreadForIO* ThreadForIO::CreateThread() {
  return new ThreadForIO();
}

ThreadForIO::ThreadForIO() {

}

ThreadForIO::~ThreadForIO() {

}

void ThreadForIO::Start() {
  if (thread_) {
    return;
  }
  started_ = true;
  thread_.reset(new std::thread(&ThreadForIO::ThreadProc, this));
  std::stringstream ss;
  ss << thread_->get_id();
  SPDLOG_INFO("Thread created, thread_id {}", ss.str());
}

void ThreadForIO::Stop() {
  SPDLOG_INFO("Thread should stop started_ {} running_ {}", started_.load(), running_.load());
  do {
    started_ = false;
    if (!running_) {
      break;
    }
    {
      std::lock_guard<std::mutex> lock(loop_guard_);
      loop_terminator_ = (uv_async_t*)malloc(sizeof(uv_async_t));
      uv_async_init(loop_, loop_terminator_, TerminateLoop);
      uv_handle_set_data((uv_handle_t*)loop_terminator_, this);
      uv_async_send(loop_terminator_);
    }
    sleep_var_.notify_one();
  } while (0);
  if (thread_) {
    std::stringstream ss;
    ss << thread_->get_id();
    SPDLOG_INFO("Thread should stop, thread_id {}", ss.str());
    if (thread_->joinable()) {
      thread_->join();
    }
  }
  assert(running_ == false);
}

uv_loop_t* ThreadForIO::Loop() {
  return loop_;
}

void ThreadForIO::ThreadProc() {
  if (!started_) {
    return;
  }
  running_ = true;
  std::stringstream ss;
  ss << thread_->get_id();
  SPDLOG_INFO("Thread running, thread_id {}", ss.str());
  loop_ = (uv_loop_t*)malloc(sizeof(uv_loop_t));
  uv_loop_init(loop_);
  int run_result = 0;
  do {
    run_result = uv_run(loop_, UV_RUN_DEFAULT);
    if (run_result == 0) {
      if (!started_) {
        break;
      }
      std::unique_lock<std::mutex> lock(loop_guard_);
      sleep_var_.wait(lock);
    }
  } while (true);
  SPDLOG_INFO("uv loop stoped run_result is {}", run_result);
  uv_loop_close(loop_);
  free(loop_);
  loop_ = nullptr;
  running_ = false;
}

void ThreadForIO::TerminateLoop(uv_async_t* handle) {
  uv_loop_t* loop = uv_handle_get_loop((uv_handle_t*)handle);
  uv_close((uv_handle_t*)handle, CleanHandle);
  uv_stop(loop);
}

void ThreadForIO::CleanHandle(uv_handle_t* handle) {
  ThreadForIO* thread = (ThreadForIO*)uv_handle_get_data(handle);
  thread->loop_terminator_ = nullptr;
  free(handle);
}