#include <libhashbuckets.h>

int main() {
  RandomZZGenerator rndgen;
  NTL::ZZ sup;
  
  std::cout << "Insert a big integer: ";
  std::cin >> sup;
  
  std::cout << "Generation of 10 random numbers less than "
            << sup << ": " << std::endl;
  rndgen.setSupremum(sup);
  for (int i = 0; i < 10; i++) {
    std::cout << (i + 1) << " - " << rndgen.next() << std::endl;
  }
  
  return 0;
}