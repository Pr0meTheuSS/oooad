#pragma once

#include <string>

#include "Processor.hpp"
#include "ProcessorFactory.hpp"

class App final {
public:
  App(int argc, char **argv);
  int run();

private:
  int mArgc;
  char **mArgv;
  std::string mProcessorName;
};
