//
// Created by wangxinshuo on 2021/2/26.
//

#include "mutex.h"

int main(){
  Mutex mutex;
  mutex.Lock();
  // do something ...

  mutex.Unlock();
  return 0;
}