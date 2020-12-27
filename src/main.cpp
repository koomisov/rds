#include <chrono>
#include <iostream>
#include <string>

#include "models/value_map.hpp"

using namespace std::chrono_literals;

int main() {
  models::ValueMap value_map;
  const std::string kKey = "1";

  value_map.set(kKey, "hello");
  std::cout << *value_map.get(kKey) << std::endl;
  const auto deleted_count = value_map.del({kKey});
  std::cout << "deleted count: " << deleted_count << std::endl;

  // -----

  value_map.set(kKey, "hello");
  std::cout << "key ttl: " << value_map.ttl(kKey)
            << ", unknown key ttl: " << value_map.ttl("123") << std::endl;

  value_map.expire(kKey, 5s);
  std::cout << "key ttl after expire: " << value_map.ttl(kKey) << std::endl;

  std::this_thread::sleep_for(2s);
  std::cout << "key ttl after 2s expire: " << value_map.ttl(kKey) << std::endl;

  std::this_thread::sleep_for(5s);
  std::cout << "key ttl after 7s expire: " << value_map.ttl(kKey) << std::endl;
  std::cout << "key existance status: "
            << value_map.get(kKey).value_or("doesn't exist") << std::endl;

  std::this_thread::sleep_for(10s);

  return 0;
}
