#include <libeopsi.h>

int main() {
  ZZPRF zzprf;
  NTL::ZZ z, seed;
  int bits;
  
  do {
    std::cout << "Insert number of bits: ";
    std::cin >> bits;
  } while (bits < 1);
  
  std::cout << "Generation of 10 random numbers with at most "
            << bits << " bits: " << std::endl;
  
  seed = zzprf.randomSeed();
  for (int i = 0; i < 10; i++) {
    z = zzprf.generate(seed, i, bits);
    std::cout << (i + 1) << " - " << z 
              << " \t(" << NTL::NumBits(z) << " bits)" << std::endl;
  }
  
  return 0;
}