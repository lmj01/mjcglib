// src/lib.cc
#include <mylib/lib.h>
#include <iostream>

namespace mylib {

void foo() {
  std::cout << "foo function" << std::endl;
//   torch::Tensor tensor = torch::rand({2, 3});
//   std::cout << tensor << std::endl;
}

void bar()
{
    std::cout << "bar function" << std::endl;
//   fmt::print("hello fmtlib\n");
}

} // namespace mylib