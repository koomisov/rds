#include "garbage_collector.hpp"

#include <chrono>
#include <thread>
#include <functional>

namespace models {

GarbageCollector::GarbageCollector(
    models::Storage& storage,
    boost::lockfree::queue<std::size_t>& expire_buckets)
    : storage_(storage),
      expire_buckets_(expire_buckets),
      task_(std::chrono::seconds(1),
            std::bind(&GarbageCollector::callback, this)) {}

void GarbageCollector::callback() {
  if (expire_buckets_.empty()) {
    return;
  }
  std::vector<std::size_t> buckets;
  buckets.reserve(16);

  std::size_t front_bucket;

  while (!expire_buckets_.empty()) {
    while (!expire_buckets_.pop(front_bucket))
      std::this_thread::yield();
    buckets.push_back(front_bucket);
  }

  std::vector<std::size_t> not_expired_buckets;
  std::vector<std::size_t> expired_buckets;
  not_expired_buckets.reserve(buckets.size());

  const auto now = std::chrono::system_clock::now();

  for (const auto bucket : buckets) {
    const auto value = storage_.get_by_bucket(bucket);
    if (!value || !value->expires_at) {
      // something goes wrong
      continue;
    }
    if (*value->expires_at < now) {
      expired_buckets.push_back(bucket);
    } else {
      not_expired_buckets.push_back(bucket);
    }
  }

  storage_.del_by_buckets(expired_buckets);

  for (const auto bucket : not_expired_buckets) {
    while (!expire_buckets_.push(bucket))
      std::this_thread::yield();
  }
}

}  // namespace models
