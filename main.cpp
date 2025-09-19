#include <CLI/CLI.hpp>
#include <fmt/core.h>
#include <iostream>
#include <memory>

#include "src/Processor.hpp"
#include "src/ProcessorFactory.hpp"

int main(int argc, char **argv) {
  CLI::App app{"Выбор процессора: квадрат или куб"};

  std::string processorName = "";
  app.add_option("-p,--processor", processorName,
                 "Processor type (square or cube)");

  CLI11_PARSE(app, argc, argv);

  fmt::print("Введите вещественное число:\n");
  double value = 0.0;
  std::cin >> value;

  auto processor = ProcessorFactory::instance().create(processorName);
  if (!processor) {
    fmt::print(stderr, "Processor '{}' не найден!\n", processorName);
    return EXIT_FAILURE;
  }

  double result = processor->process(value);
  fmt::print("Исходное значение: {0:.2f}, результат: {1:.2f}\n", value, result);

  return EXIT_SUCCESS;
}
