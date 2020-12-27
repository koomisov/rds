#include <gtest/gtest.h>
#include "test_garbage_collector.hpp"
#include "test_storage.hpp"
#include "test_value_map.hpp"

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
