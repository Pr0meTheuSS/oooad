#pragma once

#include <string>

#include "Processor.hpp"
#include "ProcessorRegistar.hpp"

constexpr const std::string cubeFactoryName{"cube"};
class CubeProcessor final : public Processor {
public:
  double process(double value) override;

private:
  static inline ProcessorRegistrar<CubeProcessor> reg{cubeFactoryName};
};
