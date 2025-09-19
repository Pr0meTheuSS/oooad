#pragma once

#include "Processor.hpp"

class CubeProcessor final : Processor {
public:
  double process(double value) override;
};
