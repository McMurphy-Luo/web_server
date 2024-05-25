#include "web_server/web_server.h"
#include <iostream>
#include <string>
#include "thread_for_io.h"
#include "uv.h"
#include "spdlog/spdlog.h"
#include "tcp_server.h"
#include "tcp_connection.h"
#include "http_server.h"
#include "async_task.h"

class CloseServer
  : public AsyncTask::Delegate
  , public RefCounter<ThreadUnsafeCounter>
{
public:
  explicit CloseServer(AsyncTask* task, HttpServer* server)
    : task_(task)
    , http_server_(server)
  {

  }

protected:
  CloseServer(const CloseServer&) = delete;
  CloseServer(CloseServer&&) = delete;
  CloseServer& operator=(const CloseServer&) = delete;
  CloseServer& operator=(CloseServer&&) = delete;
  virtual ~CloseServer() override = default;

public:
  virtual void Execute() override {
    http_server_->Stop();
  }

private:
  RefCounterPtr<AsyncTask> task_;
  RefCounterPtr<HttpServer> http_server_;
};

int main(int argc, char* argv[])
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  spdlog::set_pattern("[%Y:%T:%e %z] %^[%l] [thread:%t] [%s:%#] [%!]%$ %v");
  RefCounterPtr<ThreadForIO> thread(ThreadForIO::CreateThread());
  RefCounterPtr<HttpServer> http_server;
  {
    TcpServer* tcp_server(TcpServer::CreateTcpServer(thread.Get()));
    tcp_server->Listen(8080);
    http_server = new (std::nothrow) HttpServer(tcp_server);
    tcp_server->SetDelegate(http_server.Get());
  }
  thread->Start();
  std::string current_line;
  current_line.reserve(64);
  do {
    int readed_char = fgetc(stdin);
    if (ferror(stdin)) {
      SPDLOG_ERROR("read stdin but error. error {}", ferror(stdin));
      break;
    }
    if (feof(stdin)) {
      SPDLOG_ERROR("read stdin but end of file.");
      break;
    }
    current_line.push_back(readed_char);
    if ('\n' == readed_char) {
      if (
        current_line == "exit\n"
        ||
        current_line == "exit\r\n"
      ) {
        break;
      } else {
        current_line.clear();
      }
    }
  } while (true);
  RefCounterPtr<AsyncTask> task = AsyncTask::CreateAsyncTask(thread.Get());
  RefCounterPtr<CloseServer> closer = new CloseServer(task.Get(), http_server.Get());
  task->SetDelegate(closer.Get());
  task->Submit();
  thread->Stop();
  SPDLOG_INFO("program end.");
  return 0;
}
