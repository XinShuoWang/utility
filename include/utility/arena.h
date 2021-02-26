//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_ARENA_H_
#define UTILITY_INCLUDE_UTILITY_ARENA_H_

#include "spin_lock.h"

#include <cstdint>
#include <cstring>

class Arena {
 private:
  char *p_;
  char *start_;
  char *end_;
  uint64_t block_size_;
  SpinLock lock_;

 public:
  inline explicit Arena(uint64_t block_size){
    block_size_ = block_size;
    start_ = new char[block_size_];
    memset(start_, 0, block_size_);
    end_ = start_ + block_size_;
    p_ = start_;
  }

  ~Arena() = default;

  inline char *Allocate(uint64_t size){
    SpinLockGuard lg(&lock_);
    if (uint64_t(end_ - p_) < size) {
      start_ = new char[block_size_];
      memset(start_, 0, block_size_);
      end_ = start_ + block_size_;
      p_ = start_;
    }
    char *res = p_;
    p_ += size;
    return res;
  }
};

#endif //UTILITY_INCLUDE_UTILITY_ARENA_H_
