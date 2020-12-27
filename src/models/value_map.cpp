#include "value_map.hpp"

namespace models {

ValueMap::ValueMap()
    : expire_buckets_(16ul), garbage_collector_(storage_, expire_buckets_) {}

std::size_t ValueMap::size() const { return storage_.size(); }

std::optional<std::string> ValueMap::get(const std::string& key) const {
  const auto& value_opt = storage_.get(key);
  if (!value_opt) {
    return std::nullopt;
  }
  if (value_opt->expires_at && value_opt->expires_at < clock::now()) {
    return std::nullopt;
  }
  return value_opt ? std::make_optional(value_opt->value) : std::nullopt;
}

void ValueMap::set(const std::string& key, const std::string& value) {
  storage_.set(key, value);
}

int ValueMap::del(const std::vector<std::string>& keys) {
  return storage_.del(keys);
}

int ValueMap::expire(const std::string& key,
                     const std::chrono::seconds seconds) {
  const auto& value_opt = storage_.get(key);
  if (!value_opt) {
    return 0;
  }
  auto bucket = storage_.set(key, value_opt->value, clock::now() + seconds);

  while (!expire_buckets_.push(bucket))
    ;

  return 1;
}

int ValueMap::ttl(const std::string& key) const {
  const auto& value_opt = storage_.get(key);
  if (!value_opt) {
    return -2;
  }
  if (!value_opt->expires_at || *value_opt->expires_at < clock::now()) {
    return -1;
  }
  return std::chrono::duration_cast<std::chrono::seconds>(
             *value_opt->expires_at - clock::now())
      .count();
}

}  // namespace models
