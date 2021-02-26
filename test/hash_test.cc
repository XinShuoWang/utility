//
// Created by wangxinshuo on 2021/2/26.
//

#include "hash.h"

#include <string>
#include <iostream>

int main() {
  std::string a = "abcd";
  std::cout << Hash::Murmur(a.c_str(), a.length(), 9) << std::endl;
  std::cout << Hash::FastHash(a.c_str(), a.length()) << std::endl;
  std::cout << Hash::XXHash(a.c_str(), a.length(), 9) << std::endl;
  return 0;
}