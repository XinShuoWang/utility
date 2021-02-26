//
// Created by wangxinshuo on 2021/2/26.
//

#include "branch_prediction.h"

#include <iostream>

int main() {
  int i;
  std::cin >> i;
  if (LIKELY(i < 5)) {
    std::cout << "i < 5!" << std::endl;
  }

  if (UNLIKELY(i >= 5)) {
    std::cout << "i >= 5!" << std::endl;
  }
  return 0;
}