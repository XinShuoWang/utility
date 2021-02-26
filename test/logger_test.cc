//
// Created by wangxinshuo on 2021/2/26.
//

#include <cstdio>
#include "logger.h"

int main() {
  FILE *fp;
  fp = fopen("/tmp/test.txt", "w+");

  Logger logger(kINFO, fp);
  logger.Add(kDEBUG, "a is %d, b is %lld", 1, 2L);

  return 0;
}