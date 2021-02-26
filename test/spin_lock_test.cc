//
// Created by wangxinshuo on 2021/2/26.
//
#include "spin_lock.h"

int main() {
  SpinLock lock;

  {
    SpinLockGuard guard(&lock);
    // do something ...

  }
  return 0;
}
