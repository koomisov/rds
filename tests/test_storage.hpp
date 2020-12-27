#include <gtest/gtest.h>

#include <chrono>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "../src/models/storage.hpp"

using namespace std::chrono_literals;

TEST(Storage, test_single) {
  const std::string kKey = "1", kValue = "2";
  models::Storage storage;

  auto write_callback = [&storage, kKey, kValue]() {
    storage.set(kKey, kValue);
  };

  std::vector<std::thread> thrs;

  for (int i = 0; i < 10; ++i) {
    thrs.push_back(std::thread(write_callback));
  }

  for (auto& thr : thrs) {
    thr.detach();
  }

  ASSERT_EQ(storage.size(), 1);
  ASSERT_EQ(storage.get(kKey), kValue);

  const auto deleted_count = storage.del({kKey, "2", "3"});
  ASSERT_EQ(deleted_count, 1);
  ASSERT_EQ(storage.size(), 0);
  ASSERT_EQ(storage.get(kKey), std::nullopt);
}
