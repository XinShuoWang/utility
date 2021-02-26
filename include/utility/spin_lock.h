//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_SPIN_LOCK_H_
#define UTILITY_INCLUDE_UTILITY_SPIN_LOCK_H_

#include <pthread.h>

class SpinLock {
 private:
  pthread_spinlock_t lock_;

 public:
  inline SpinLock() { pthread_spin_init(&lock_, PTHREAD_PROCESS_PRIVATE); }

  inline ~SpinLock() { pthread_spin_destroy(&lock_); }

  inline void Lock() { pthread_spin_lock(&lock_); }

  inline void Unlock() { pthread_spin_unlock(&lock_); }
};

class SpinLockGuard {
 private:
  SpinLock *lock_;

 public:
  inline SpinLockGuard(SpinLock *lock) {
    lock_ = lock;
    lock_->Lock();
  }

  inline ~SpinLockGuard() { lock_->Unlock(); }
};

#endif //UTILITY_INCLUDE_UTILITY_SPIN_LOCK_H_
