# TaskScheduler

## Описание

`TaskScheduler` — это простой многопоточный планировщик задач на языке C++. Он принимает задачи, которые должны быть выполнены не раньше определённого времени, и обрабатывает их с использованием заданного количества потоков.

## Пример

```cpp
#include "TaskScheduler.h"
#include <ctime>

void exampleTask(int id) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "task " << id << " done: " << std::time(nullptr) << std::endl;
}

int main() {
    size_t numThreads = 4; 
    TaskScheduler scheduler(numThreads);

    
    for (int i = 0; i < 5; ++i) {
        scheduler.Add([i] { exampleTask(i); }, std::time(nullptr) + i * 2); 
    }

    std::this_thread::sleep_for(std::chrono::seconds(10)); 
    return 0;
}
```

## Инструкции по сборке

Для сборки проекта потребуется компилятор с поддержкой C++11 или выше и флаг `-lpthread` для использования многопоточности.

1. Компиляция:

   ```bash
   g++ main.cpp TaskScheduler.cpp -o scheduler -lpthread
   ```

2. Запуск:

   ```bash
   ./scheduler
   ```
