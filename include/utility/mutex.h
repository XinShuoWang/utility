//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_MUTEX_H_
#define UTILITY_INCLUDE_UTILITY_MUTEX_H_

#if __cplusplus >= 199711L

#include <mutex>

class Mutex {
 private:
  std::mutex m;
 public:
  inline void Lock() { m.lock(); }
  inline void Unlock() { m.unlock(); }
};

#else

#include <pthread.h>

class Mutex {
 private:
  pthread_mutex_t mutex_{};
 public:
  inline Mutex() {
    mutex_ = PTHREAD_MUTEX_INITIALIZER;
  }

  inline void Lock() {
    pthread_mutex_lock(&mutex_);
  }

  inline void Unlock() {
    pthread_mutex_unlock(&mutex_);
  }
};

#endif

#endif //UTILITY_INCLUDE_UTILITY_MUTEX_H_
