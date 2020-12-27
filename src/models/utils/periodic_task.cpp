#include "periodic_task.hpp"

namespace utils {

PeriodicTask::PeriodicTask(const std::chrono::milliseconds period,
                           const std::function<void()>& callback)
    : period_(period), callback_(callback), is_running_(true) {
  thread_ = std::thread([this] {
    while (is_running_) {
      std::this_thread::sleep_for(period_);
      if (is_running_) {
        callback_();
      }
    }
  });
}

PeriodicTask::~PeriodicTask() {
  is_running_ = false;
  thread_.join();
}

}  // namespace utils
