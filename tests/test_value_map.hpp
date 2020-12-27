#include <gtest/gtest.h>

#include <chrono>
#include <optional>
#include <string>
#include <thread>
#include <vector>

#include "../src/models/value_map.hpp"

using namespace std::chrono_literals;

TEST(ValueMap, test_single) {
  const std::string kValue = "hello";
  models::ValueMap value_map;

  value_map.set("1", kValue);
  value_map.set("2", kValue);
  value_map.set("3", kValue);

  ASSERT_EQ(value_map.size(), 3);
  ASSERT_EQ(value_map.get("1"), kValue);
  ASSERT_EQ(value_map.get("2"), kValue);
  ASSERT_EQ(value_map.get("3"), kValue);

  const auto deleted_count = value_map.del({"1", "2", "3"});
  ASSERT_EQ(deleted_count, 3);
  ASSERT_EQ(value_map.size(), 0);

  value_map.set("1", kValue);
  value_map.set("2", kValue);
  value_map.expire("1", 0s);
  ASSERT_LE(value_map.ttl("1"), 0);
  std::this_thread::sleep_for(10ms);

  ASSERT_EQ(value_map.get("1"), std::nullopt);
  ASSERT_LT(value_map.ttl("1"), 0);
  ASSERT_EQ(value_map.get("2"), kValue);
  ASSERT_EQ(value_map.ttl("2"), -1);
}
