#include "TaskScheduler.h"

TaskScheduler::TaskScheduler(size_t numThreads) : stop(false) {
  for (size_t i = 0; i < numThreads; ++i) {
    workers.emplace_back([this] { this->run(); });
  }
}

TaskScheduler::~TaskScheduler() {
  {
    std::lock_guard<std::mutex> lock(mtx);
    stop = true;
  }
  cv.notify_all();
  for (std::thread& worker : workers) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void TaskScheduler::Add(std::function<void()> task, std::time_t timestamp) {
  std::lock_guard<std::mutex> lock(mtx);
  tasks.emplace(timestamp, std::move(task));
  cv.notify_one();
}

void TaskScheduler::run() {
  while (true) {
    std::unique_lock<std::mutex> lock(mtx);

    if (stop && tasks.empty()) return;

    if (!tasks.empty()) {
      auto now = std::time(nullptr);
      auto& nextTask = tasks.top();

      if (nextTask.timestamp <= now) {
        auto task = std::move(nextTask.task);
        tasks.pop();
        lock.unlock();
        task();
      } else {
        cv.wait_until(
            lock, std::chrono::system_clock::from_time_t(nextTask.timestamp));
      }
    } else {
      cv.wait(lock);
    }
  }
}
