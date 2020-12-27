#pragma once

#include <chrono>
#include <optional>
#include <string>

namespace models {

struct Value {
  Value() = default;
  Value(const Value&) = default;

  Value(std::string value,
        std::optional<std::chrono::system_clock::time_point> expires_at =
            std::nullopt)
      : value(std::move(value)), expires_at(expires_at) {}

  std::string value;
  std::optional<std::chrono::system_clock::time_point> expires_at;

  bool operator==(const Value& other) const {
    return this->value == other.value;
  }
};

}  // namespace models
