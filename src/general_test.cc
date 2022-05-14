#include <iostream>

int main(int argc, char* argv[]) {
  int32_t num = 0u;

  num <<= 1;
  num += 1;
  num <<= 1;
  

  std::cout << "num: " << num << std::endl;
}