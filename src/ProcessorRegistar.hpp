#pragma once

#include <memory>
#include <string>

#include "Processor.hpp"
#include "ProcessorFactory.hpp"

template <typename T> struct ProcessorRegistrar {
  ProcessorRegistrar(const std::string &name) {
    ProcessorFactory::instance().registerProcessor(
        name, []() -> std::unique_ptr<Processor> {
          return std::unique_ptr<Processor>(std::make_unique<T>().release());
        });
  }
};
