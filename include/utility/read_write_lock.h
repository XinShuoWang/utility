//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_READ_WRITE_LOCK_H_
#define UTILITY_INCLUDE_UTILITY_READ_WRITE_LOCK_H_

#include <pthread.h>

class ReadWriteLock {
 private:
  pthread_rwlock_t lock_;

 public:
  inline ReadWriteLock() { pthread_rwlock_init(&lock_, nullptr); }
  inline ~ReadWriteLock() { pthread_rwlock_destroy(&lock_); }
  inline void ReaderLock() { pthread_rwlock_rdlock(&lock_); }
  inline void ReaderUnLock() { pthread_rwlock_unlock(&lock_); }
  inline void WriterLock() { pthread_rwlock_wrlock(&lock_); }
  inline void WriterUnLock() { pthread_rwlock_unlock(&lock_); }
  ReadWriteLock(const ReadWriteLock &src) = delete;
  ReadWriteLock &operator=(const ReadWriteLock &rhs) = delete;
};

class ReadLockGuard {
 private:
  ReadWriteLock *lock_;

 public:
  inline explicit ReadLockGuard(ReadWriteLock *lock) {
    lock_ = lock;
    lock_->ReaderLock();
  }

  inline ~ReadLockGuard() { lock_->ReaderUnLock(); }
};

class WriteLockGuard {
 private:
  ReadWriteLock *lock_;

 public:
  inline explicit WriteLockGuard(ReadWriteLock *lock) {
    lock_ = lock;
    lock_->WriterLock();
  }

  inline ~WriteLockGuard() { lock_->WriterUnLock(); }
};

#endif //UTILITY_INCLUDE_UTILITY_READ_WRITE_LOCK_H_
