#include "AppConfiguration.hpp"

void AppConfiguration::addProperty(const std::string &key,
                                   const std::string &value) {
  properties_[key] = value;
}

std::optional<std::string>
AppConfiguration::getProperty(const std::string &key) const {
  if (auto it = properties_.find(key); it != properties_.end()) {
    return it->second;
  }
  return std::nullopt;
}

std::string AppConfiguration::getPropertyOr(const std::string &key,
                                            const std::string &def) const {
  if (auto it = properties_.find(key); it != properties_.end()) {
    return it->second;
  }
  return def;
}
