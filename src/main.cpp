#include "web_server/web_server.h"
#include <iostream>
#include <string>
#include "thread_for_io.h"
#include "uv.h"
#include "spdlog/spdlog.h"
#include "tcp_server.h"

int main(int argc, char* argv[])
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  spdlog::set_pattern("[%Y:%T:%e %z] %^[%l] [thread:%t] [%s:%#] [%!]%$ %v");
  RefCounterPtr<ThreadForIO> thread(ThreadForIO::CreateThread());
  thread->Start();
  RefCounterPtr<TcpServer> server(TcpServer::CreateTcpServer(thread.Get()));
  server->Listen(8080);
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
  thread->Stop();
  SPDLOG_INFO("program end.");
  return 0;
}
