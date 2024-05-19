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
    uv_stop(loop_);
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
  while (started_) {
    run_result = uv_run(loop_, UV_RUN_DEFAULT);
  }
  SPDLOG_INFO("uv loop stoped run_result is {}", run_result);
  uv_loop_close(loop_);
  free(loop_);
  loop_ = nullptr;
  running_ = false;
}
