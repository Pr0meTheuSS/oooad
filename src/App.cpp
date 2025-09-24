#include "App.hpp"

#include <iostream>

#include <CLI/CLI.hpp>
#include <fmt/core.h>

App::App(int argc, char **argv) : mArgc(argc), mArgv(argv) {}

int App::run() {
  CLI::App cliApp{"Выбор процессора: квадрат или куб"};
  auto processorOption = cliApp.add_option("-p,--processor", mProcessorName,
                                           "Processor type (square or cube)");
  processorOption->required(true);

  CLI11_PARSE(cliApp, mArgc, mArgv);

  fmt::print("Введите вещественное число:\n");
  double value = 0.0;
  std::cin >> value;

  auto processor = ProcessorFactory::instance().create(mProcessorName);
  if (!processor) {
    fmt::print(stderr, "Processor '{}' не найден!\n", mProcessorName);
    return EXIT_FAILURE;
  }

  double result = processor->process(value);
  fmt::print("Исходное значение: {0:.2f}, результат: {1:.2f}\n", value, result);

  return EXIT_SUCCESS;
}
