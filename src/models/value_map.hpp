#pragma once

#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/lockfree/queue.hpp>

#include "garbage_collector.hpp"
#include "storage.hpp"

namespace models {

class ValueMap {
 public:
  ValueMap();
  virtual ~ValueMap() = default;

  std::optional<std::string> get(const std::string& key) const;
  void set(const std::string& key, const std::string& value);
  int del(const std::vector<std::string>& keys);

  int expire(const std::string& key, const std::chrono::seconds seconds);
  int ttl(const std::string& key) const;

 private:
  using clock = std::chrono::system_clock;

  models::Storage storage_;
  boost::lockfree::queue<std::size_t> expire_buckets_;
  models::GarbageCollector garbage_collector_;
};

}  // namespace models
