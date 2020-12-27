#include "storage.hpp"

namespace models {

Storage::Storage() { storage_.max_load_factor(1.f); }

int Storage::del(const std::vector<std::string>& keys) {
  int deleted_count = 0;
  std::unique_lock<std::shared_mutex> lock(mutex_);
  for (const auto& key : keys) {
    deleted_count += storage_.erase(key);
  }
  return deleted_count;
}

int Storage::del_by_buckets(const std::vector<std::size_t>& buckets) {
  int deleted_count = 0;
  std::unique_lock<std::shared_mutex> lock(mutex_);
  for (const auto& bucket : buckets) {
    for (auto it = storage_.begin(bucket); it != storage_.end(bucket); ++it) {
      deleted_count += storage_.erase(it->first);
    }
  }
  return deleted_count;
}

std::optional<models::Value> Storage::get(const std::string& key) const {
  std::shared_lock<std::shared_mutex> lock(mutex_);
  const auto it = storage_.find(key);
  if (it == storage_.end()) {
    return std::nullopt;
  }
  return it->second;
}

std::optional<models::Value> Storage::get_by_bucket(
    const std::size_t bucket) const {
  std::shared_lock<std::shared_mutex> lock(mutex_);
  const auto it = storage_.begin(bucket);
  if (it == storage_.end(bucket)) {
    return std::nullopt;
  }
  return it->second;
}

int Storage::set(
    const std::string& key, const std::string& value,
    std::optional<std::chrono::system_clock::time_point> expires_at) {
  std::unique_lock<std::shared_mutex> lock(mutex_);
  storage_[key] = models::Value(value, expires_at);
  return storage_.bucket(key);
}

}  // namespace models
