#pragma once

#include <boost/lockfree/queue.hpp>

#include "storage.hpp"
#include "utils/periodic_task.hpp"

namespace models {

class GarbageCollector {
 public:
  explicit GarbageCollector(
      models::Storage& storage,
      boost::lockfree::queue<std::size_t>& expire_buckets);

  void callback();

 private:
  models::Storage& storage_;
  boost::lockfree::queue<std::size_t>& expire_buckets_;
  utils::PeriodicTask task_;
};

}  // namespace models
