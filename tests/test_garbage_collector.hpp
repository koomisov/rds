#include <gtest/gtest.h>

#include <chrono>
#include <optional>
#include <string>
#include <thread>

#include "../src/models/value_map.hpp"

using namespace std::chrono_literals;

TEST(GarbageCollector, test_single) {
  const std::string kKey = "1", kValue = "hello";

  models::ValueMap value_map;
  value_map.set(kKey, kValue);
  ASSERT_EQ(value_map.size(), 1);

  value_map.expire(kKey, 0s);
  std::this_thread::sleep_for(1s);
  ASSERT_EQ(value_map.get(kKey), std::nullopt);
  std::this_thread::sleep_for(1s);

  ASSERT_EQ(value_map.get(kKey), std::nullopt);
  ASSERT_LT(value_map.ttl(kKey), 0);
  ASSERT_EQ(value_map.size(), 0);
  ASSERT_EQ(value_map.expire(kKey, 1s), 0);
}

TEST(GarbageCollector, test_many) {
  const std::string kValue = "hello";

  models::ValueMap value_map;
  value_map.set("1", kValue);
  ASSERT_EQ(value_map.size(), 1);

  value_map.expire("1", 1s);
  value_map.set("2", kValue);
  ASSERT_GE(value_map.size(), 1);
  std::this_thread::sleep_for(2s);

  ASSERT_EQ(value_map.size(), 1);
  ASSERT_EQ(value_map.get("1"), std::nullopt);

  value_map.set("1", kValue);
  value_map.set("2", kValue);
  value_map.set("3", kValue);
  value_map.expire("1", 1s);
  value_map.expire("2", 1s);
  value_map.expire("3", 1s);

  std::this_thread::sleep_for(2s);

  ASSERT_EQ(value_map.get("3"), std::nullopt);
  ASSERT_LT(value_map.ttl("3"), 0);
  ASSERT_EQ(value_map.size(), 0);
  ASSERT_EQ(value_map.expire("3", 1s), 0);
}
