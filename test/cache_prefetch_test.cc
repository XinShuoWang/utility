//
// Created by wangxinshuo on 2021/2/28.
//

#include "cache_prefetch.h"

int main() {
  char data[] = {'a', 'b', 'c', 'd'};
  CachePrefetch::Load(data);
  CachePrefetch::LoadInAllLevelExceptL0AndL1(data);
  CachePrefetch::LoadInAllLevelExceptL0(data);
  CachePrefetch::LoadInAllLevel(data);
  return 0;
}