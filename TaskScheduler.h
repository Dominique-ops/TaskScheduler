#ifndef TASKSCHEDULER_H
#define TASKSCHEDULER_H

#include <chrono>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class TaskScheduler {
 public:
  explicit TaskScheduler(size_t numThreads);
  ~TaskScheduler();

  void Add(std::function<void()> task, std::time_t timestamp);

 private:
  struct Task {
    std::time_t timestamp;
    std::function<void()> task;
    Task(std::time_t ts, std::function<void()> t)
        : timestamp(ts), task(std::move(t)) {}
    bool operator<(const Task& other) const {
      return timestamp > other.timestamp;
    }
  };

  void run();

  std::priority_queue<Task> tasks;
  std::mutex mtx;
  std::condition_variable cv;
  std::vector<std::thread> workers;
  bool stop;
};

#endif
