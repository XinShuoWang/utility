//
// Created by wangxinshuo on 2021/2/26.
//

#include "thread_pool.h"

void wait(int seconds){
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

int main(){
  ThreadPool thread_pool(8);
  thread_pool.AddJob(std::function<wait>());
  return 0;
}