#pragma once

#include <string>

#include "Processor.hpp"
#include "ProcessorRegistar.hpp"

constexpr const std::string squareFactoryName{"square"};
class SquareProcessor final : public Processor {
public:
  double process(double value) override;

private:
  static inline ProcessorRegistrar<SquareProcessor> reg{squareFactoryName};
};
