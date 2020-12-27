#pragma once

#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

#include "value.hpp"

namespace models {

class Storage {
 public:
  explicit Storage();
  virtual ~Storage() = default;

  std::size_t size() const;

  std::optional<models::Value> get_by_bucket(const std::size_t bucket) const;

  std::optional<models::Value> get(const std::string& key) const;
  int set(const std::string& key, const std::string& value,
          std::optional<std::chrono::system_clock::time_point> expires_at =
              std::nullopt);

  int del_by_buckets(const std::vector<std::size_t>& buckets);
  int del(const std::vector<std::string>& keys);

 private:
  std::unordered_map<std::string, models::Value> storage_;
  mutable std::shared_mutex mutex_;
};

}  // namespace models
