//
// Created by wangxinshuo on 2021/2/26.
//

#include "cpu_affinity.h"

#include <iostream>

int main(){
  CpuAffinity affinity(4);
  pthread_t thread = pthread_self();
  affinity.SetAffinity(thread);
  std::cout << affinity.GetAffinity(thread) << std::endl;
  return 0;
}