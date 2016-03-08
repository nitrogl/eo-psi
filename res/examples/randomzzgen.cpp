#include <libhashbuckets.h>

int main() {
  ZZPRF zzprf;
  NTL::ZZ z, seed;
  size_t bits;
  
  std::cout << "Insert the (maximum) number of bits for each number to generate: ";
  std::cin >> bits;
  
  std::cout << "Generation of 10 random numbers with at most "
            << bits << " bits: " << std::endl;
  
  seed = zzprf.randomSeed();
  for (size_t i = 0; i < 10; i++) {
    z = zzprf.generate(seed, i, bits);
    std::cout << (i + 1) << " - " << z << "\t(" << NTL::NumBits(z) << " bits)" << std::endl;
  }
  
  return 0;
}