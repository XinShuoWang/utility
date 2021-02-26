//
// Created by wangxinshuo on 2021/2/26.
//

#include "arena.h"

#include <iostream>

class Example {
 private:
  int a;
  uint64_t b;

 public:
  Example() {
    a = 0;
    b = 0;
  }

  ~Example() = default;

  void to_string() const {
    std::cout << "a = " << a << ", b = " << b << "." << std::endl;
  }
};

int main() {
  Arena arena(10240);
  void *p = arena.Allocate(sizeof(Example));
  // init object
  auto *example = new(p) Example;
  example->to_string();
}