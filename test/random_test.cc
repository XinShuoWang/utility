//
// Created by wangxinshuo on 2021/3/15.
//
#include "random.h"

#include <iostream>

int main(){
  Random random(345);
  for (int i = 0; i < 1000; ++i) {
    std::cout << random.Next() << std::endl;
  }
  return 0;
}
