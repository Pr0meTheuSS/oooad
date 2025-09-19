#pragma once

class Processor {
public:
  Processor() = default;
  virtual ~Processor() = default;

   virtual double process(double) = 0;
};
