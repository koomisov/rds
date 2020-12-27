#include <iostream>

#include "models/value_map.hpp"

using namespace std::chrono_literals;

int main() {
  models::ValueMap value_map;
  value_map.set("key", "Hello, World!");
  std::cout << *value_map.get("key") << std::endl;
  return 0;
}
