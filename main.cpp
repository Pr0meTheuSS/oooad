#include <iostream>

#include <fmt/core.h>

#include "src/SquareProcessor.hpp"

int main() {
  fmt::print("Введите вещественное число для возведения его в квадрат:\n");

  double value = 0.0;
  std::cin >> value;

  SquareProcessor squareProcessor;
  Processor &processor = squareProcessor;

  double result = processor.process(value);
  fmt::print("Исходное значение: {0:.2f} квадрат значения: {1:.2f}\n", value,
             result);

  return EXIT_SUCCESS;
}
