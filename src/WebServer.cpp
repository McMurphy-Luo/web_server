#include "Windows.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
  for (int i = 0; i < argc; ++i) {
    std::cout << "command line arg " << i << ": ";
    std::cout << argv[i];
    std::cout << std::endl;
  }
}
