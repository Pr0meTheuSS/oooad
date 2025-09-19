#pragma once

#include <optional>
#include <string>
#include <unordered_map>

class AppConfiguration final {
public:
  AppConfiguration() = default;

  void addProperty(const std::string &key, const std::string &value);

  std::optional<std::string> getProperty(const std::string &key) const;

  std::string getPropertyOr(const std::string &key,
                            const std::string &def) const;

private:
  std::unordered_map<std::string, std::string> properties_;
};
