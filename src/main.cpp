#include <iostream>

#include <fmt/core.h>

int main() {
  fmt::print("Введите вещественное число для возведения его в квадрат:\n");

  double value = 0.0;
  std::cin >> value;

  double square = value * value;
  fmt::print("Исходное значение: {0:.2f} квадрат значения: {1:.2f}\n", value, square);

  return EXIT_SUCCESS;
}
