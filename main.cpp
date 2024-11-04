
#include <ctime>

#include "TaskScheduler.h"

void exampleTask(int id) {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "task " << id << " done: " << std::time(nullptr) << std::endl;
}

int main() {
  size_t numThreads = 4;
  TaskScheduler scheduler(numThreads);

  for (int i = 0; i < 8; ++i) {
    scheduler.Add([i] { exampleTask(i); }, std::time(nullptr) + i * 2);
  }

  std::this_thread::sleep_for(std::chrono::seconds(10));
  return 0;
}
