//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_MUTEX_H_
#define UTILITY_INCLUDE_UTILITY_MUTEX_H_

#include <mutex>

class Mutex {
 private:
  std::mutex m;
 public:
  inline void Lock() { m.lock(); }
  inline void Unlock() { m.unlock(); }
};

#endif //UTILITY_INCLUDE_UTILITY_MUTEX_H_
