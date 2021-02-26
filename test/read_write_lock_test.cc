//
// Created by wangxinshuo on 2021/2/26.
//

#include "read_write_lock.h"

int main() {
  ReadWriteLock lock;

  {
    ReadLockGuard guard(&lock);
    // do something ...

  }

  {
    WriteLockGuard guard(&lock);
    // do something ...

  }

  return 0;
}