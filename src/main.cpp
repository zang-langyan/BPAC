#include <stdio.h>
#include "Window.h"

#include <string>
#include <poppler-document.h>


int main(int, char**)
{
  std::cout << "Welcome!\n";
  Window BPAC(2560, 1280, "BPAC++");
  BPAC.run();

  return 0;
}