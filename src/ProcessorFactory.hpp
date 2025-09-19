#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "Processor.hpp"

class ProcessorFactory {
public:
  using Creator = std::function<std::unique_ptr<Processor>()>;

  static ProcessorFactory &instance() {
    static ProcessorFactory inst;
    return inst;
  }

  void registerProcessor(const std::string &name, Creator creator) {
    registry_[name] = std::move(creator);
  }

  std::unique_ptr<Processor> create(const std::string &name) {
    auto it = registry_.find(name);
    if (it != registry_.end()) {
      return it->second();
    }
    return nullptr;
  }

private:
  std::map<std::string, Creator> registry_;
};
