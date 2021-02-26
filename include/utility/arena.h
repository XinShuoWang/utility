//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_ARENA_H_
#define UTILITY_INCLUDE_UTILITY_ARENA_H_

#include "spin_lock.h"

#include <cstdint>
#include <cstring>

class Arena{
 private:
  char *p_;
  char *start_;
  char *end_;
  uint64_t block_size_;
  SpinLock lock_;

 public:
  explicit Arena(uint64_t block_size);

  ~Arena() = default;

  char* Allocate(uint64_t size);
};

#endif //UTILITY_INCLUDE_UTILITY_ARENA_H_
