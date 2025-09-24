#pragma once

#include <string>

#include "Processor.hpp"
#include "ProcessorRegistar.hpp"

class SquareProcessor final : public Processor {
public:
  double process(double value) override;
};

constexpr const std::string squareFactoryName{"square"};
namespace {
static inline ProcessorRegistrar<SquareProcessor> regSquareProcessor{squareFactoryName};
}
