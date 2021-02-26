//
// Created by wangxinshuo on 2021/2/26.
//

#include "thread_pool.h"

#include <iostream>

int main() {
  ThreadPool thread_pool(8);
  // function pointer
  std::function<void()> task_runner = []() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Thread " << std::this_thread::get_id() << " Complete task!" << std::endl;
  };
  thread_pool.AddJob(task_runner);
  // it will wait all run complete!
  return 0;
}