#pragma once

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

namespace utils {

class PeriodicTask {
 public:
  explicit PeriodicTask(const std::chrono::milliseconds period,
                        const std::function<void()>& func);
  virtual ~PeriodicTask();

 private:
  std::chrono::milliseconds period_;
  std::function<void()> callback_;
  std::atomic<bool> is_running_;
  std::thread thread_;
};

}  // namespace utils
