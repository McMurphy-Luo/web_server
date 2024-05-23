#include "thread_for_io.h"
#include <sstream>
#include "uv.h"
#include "spdlog/spdlog.h"

ThreadForIO* ThreadForIO::CreateThread() {
  return new ThreadForIO();
}

ThreadForIO::ThreadForIO() {
  if (!loop_) {
    loop_ = (uv_loop_t*)malloc(sizeof(uv_loop_t));
  }
  if (loop_) {
    uv_loop_init(loop_);
  }
}

ThreadForIO::~ThreadForIO() {
  free(loop_);
  loop_ = nullptr;
}

void ThreadForIO::Start() {
  if (thread_) {
    return;
  }
  thread_.reset(new (std::nothrow) std::thread(&ThreadForIO::ThreadProc, this));
  std::stringstream ss;
  ss << "thread_id " << thread_->get_id() << " this_thread_id " << std::this_thread::get_id();
  SPDLOG_INFO("Thread created, {}", ss.str());
}

void ThreadForIO::Stop() {
  std::stringstream ss;
  if (thread_) {
    ss << thread_->get_id();
  }
  SPDLOG_INFO("Thread should stop, has_thread {}, thread_id {}", !!thread_, ss.str());
  if (loop_) {
    std::lock_guard<std::mutex> lock(loop_guard_);
    should_terminate_ = true;
    sleep_var_.notify_one();
  }
  if (thread_) {
    std::stringstream ss;
    ss << thread_->get_id();
    if (thread_->joinable()) {
      thread_->join();
      SPDLOG_INFO("Thread stopped, thread_id {}", ss.str());
      thread_.reset();
    }
  }
}

uv_loop_t* ThreadForIO::Handle() {
  return loop_;
}

void ThreadForIO::Wake() {
  std::lock_guard<std::mutex> lock(loop_guard_);
  sleep_var_.notify_one();
}

void ThreadForIO::ThreadProc() {
  std::stringstream ss;
  ss << std::this_thread::get_id();
  SPDLOG_INFO("Thread started, this_thread_id {}", ss.str());
  int run_result = 0;
  do {
    run_result = uv_run(loop_, UV_RUN_DEFAULT);
    if (run_result == 0) {
      if (should_terminate_) {
        break;
      }
      std::unique_lock<std::mutex> lock(loop_guard_);
      sleep_var_.wait(lock);
    }
  } while (true);
  int close_result = uv_loop_close(loop_);
  SPDLOG_INFO("close_result {}", close_result);
  assert(UV_EBUSY != close_result);
}
