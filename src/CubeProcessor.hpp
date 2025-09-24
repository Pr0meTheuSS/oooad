#pragma once

#include <string>

#include "Processor.hpp"
#include "ProcessorRegistar.hpp"

class CubeProcessor final : public Processor {
public:
  double process(double value) override;

};

constexpr const std::string cubeFactoryName{"cube"};
namespace {
  static inline ProcessorRegistrar<CubeProcessor> regCubeProcessor{cubeFactoryName};
}