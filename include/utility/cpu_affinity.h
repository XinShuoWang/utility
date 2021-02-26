//
// Created by wangxinshuo on 2021/2/26.
//

#ifndef UTILITY_INCLUDE_UTILITY_CPU_AFFINITY_H_
#define UTILITY_INCLUDE_UTILITY_CPU_AFFINITY_H_

#include <pthread.h>
#include <cstdio>
#include <cstdlib>

class CpuAffinity{
 private:
  cpu_set_t cpu_set_;
 public:
  inline CpuAffinity(int cores=8){
    // add all cpu to set.
    CPU_ZERO(&cpu_set_);
    for (int i = 0; i < cores; i++)
      CPU_SET(i, &cpu_set_);
  }

  inline bool SetAffinity(pthread_t& thread){
    return pthread_setaffinity_np(thread, sizeof(cpu_set_), &cpu_set_) == 0;
  }

  inline int GetAffinity(pthread_t& thread){
    return pthread_getaffinity_np(thread, sizeof(cpu_set_), &cpu_set_);
  }

  inline bool InSet(int i){
    return CPU_ISSET(i, &cpu_set_);
  }
};

#endif //UTILITY_INCLUDE_UTILITY_CPU_AFFINITY_H_
