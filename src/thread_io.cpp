#include "thread_io.h"
#include <sstream>
#include "uv.h"
#include "spdlog/spdlog.h"

ThreadIO::ThreadIO() {

}

void ThreadIO::Start() {
  if (thread_) {
    return;
  }
  started_ = true;
  thread_.reset(new std::thread(&ThreadIO::ThreadProc, this));
  std::stringstream ss;
  ss << thread_->get_id();
  SPDLOG_INFO("Thread created, thread_id {}", ss.str());
}

void ThreadIO::Stop() {
  do {
    started_ = false;
    if (!running_) {
      break;
    }
    uv_stop(loop_);
  } while (0);
  if (thread_) {
    if (thread_->joinable()) {
      thread_->join();
    }
  }
  assert(running_ == false);
}

void ThreadIO::ThreadProc() {
  if (started_) {
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
