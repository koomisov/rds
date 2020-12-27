#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#include "models/value_map.hpp"

using namespace std::chrono_literals;

const std::string kValue = "Hello!";

class TimeMeasure {
 public:
  TimeMeasure() : start_(std::chrono::system_clock::now()) {}

  int get_time() const {
    return std::chrono::duration_cast<std::chrono::microseconds>(
               std::chrono::system_clock::now() - start_)
        .count();
  }

 private:
  std::chrono::system_clock::time_point start_;
};

int set(models::ValueMap& vm) {
  auto key = std::to_string(rand() % 10000000);

  TimeMeasure tm;
  vm.set(key, kValue);
  return tm.get_time();
}

int get(models::ValueMap& vm) {
  auto key = std::to_string(rand() % 10000000);

  TimeMeasure tm;
  vm.get(key);
  return tm.get_time();
}

int del(models::ValueMap& vm) {
  auto key = std::to_string(rand() % 10000000);

  TimeMeasure tm;
  vm.del({key});
  return tm.get_time();
}

int expire(models::ValueMap& vm) {
  auto key = std::to_string(rand() % 10000000);

  TimeMeasure tm;
  vm.set(key, kValue);
  vm.expire(key, std::chrono::seconds(1));
  return tm.get_time();
}

int main() {
  srand(time(NULL));

  alignas(128) models::ValueMap vm;
  alignas(128) std::atomic<bool> finished = false;
  alignas(128) std::atomic<std::size_t> sum_read_time = 0, read_operations = 0,
                                        sum_write_time = 0,
                                        write_operations = 0;

  std::thread get_thread([&vm, &finished, &sum_read_time, &read_operations]() {
    while (!finished) {
      for (int i = 0; i < 1000; ++i) {
        auto time = get(vm);
        sum_read_time += time;
        ++read_operations;
      }
    }
  });

  std::thread set_thread(
      [&vm, &finished, &sum_write_time, &write_operations]() {
        while (!finished) {
          for (int i = 0; i < 1000; ++i) {
            auto time = set(vm);
            std::this_thread::yield();
            sum_write_time += time;
            ++write_operations;
          }
        }
      });

  std::thread expire_thread(
      [&vm, &finished, &sum_write_time, &write_operations]() {
        while (!finished) {
          for (int i = 0; i < 1000; ++i) {
            auto time = expire(vm);
            sum_write_time += time;
            ++write_operations;
          }
          std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
      });

  std::this_thread::sleep_for(std::chrono::seconds(10));
  finished = true;
  expire_thread.join();
  get_thread.join();
  set_thread.join();

  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::cout << "Timings in microseconds: " << std::endl;

  std::cout << "read operations: " << read_operations
            << " , avg read time: " << sum_read_time / read_operations
            << std::endl;
  std::cout << "write operations: " << write_operations
            << " , avg write time: " << sum_write_time / write_operations
            << std::endl;

  return 0;
}
