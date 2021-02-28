//
// Created by wangxinshuo on 2021/2/28.
//

#ifndef UTILITY_INCLUDE_UTILITY_CACHE_PREFETCH_H_
#define UTILITY_INCLUDE_UTILITY_CACHE_PREFETCH_H_

#include <xmmintrin.h>

class CachePrefetch {
 public:
  static void LoadInAllLevel(char *addr) {
    _mm_prefetch(addr, _MM_HINT_T0);
  }

  static void LoadInAllLevelExceptL0(char *addr) {
    _mm_prefetch(addr, _MM_HINT_T1);
  }

  static void LoadInAllLevelExceptL0AndL1(char *addr) {
    _mm_prefetch(addr, _MM_HINT_T2);
  }

  // 预取数据到非临时缓冲结构中，可以最小化对缓存的污染。
  static void Load(char *addr) {
    _mm_prefetch(addr, _MM_HINT_NTA);
  }
};

#endif //UTILITY_INCLUDE_UTILITY_CACHE_PREFETCH_H_
