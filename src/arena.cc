//
// Created by wangxinshuo on 2021/2/26.
//
#include "arena.h"

Arena::Arena(uint64_t block_size) {
  block_size_ = block_size;
  start_ = new char[block_size_];
  memset(start_, 0, block_size_);
  end_ = start_ + block_size_;
  p_ = start_;
}

char *Arena::Allocate(uint64_t size) {
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