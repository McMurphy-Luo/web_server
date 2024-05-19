#include "web_server/web_server.h"
#include <iostream>
#include <string>
#include "thread_io.h"
#include "uv.h"
#include "spdlog/spdlog.h"

int main(int argc, char* argv[])
{
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  spdlog::set_pattern("[%Y:%T:%e %z] %^[%l] [thread:%t] [%s:%#] [%!]%$ %v");
  ThreadIO t;
  t.Start();
  t.Stop();
  return 0;
}
