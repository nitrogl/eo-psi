#include <iostream>
#include <cstdlib>
#include <fstream>
#include <NTL/ZZ.h>
#include "test.h"
//-----------------------------------------------------------------------------

int main(int argc, char **argv) {
  NTL::ZZ p;
  std::string seed;
  std::ofstream outfile;
  size_t n = DEFAULT_N;
  std::string pstr = DEFAULT_P;
  std::string outfilename = DEFAULT_FILENAME;
  
  p = str2zz(pstr);
  
  // Initialize random seed
  srand(time(NULL));
  
  // Generate a random seed
  for (int i = 0; i <= rand() % SEED_MAX_LENGTH; i++) {
    seed.append(1, rand() % 10 + '0');
  }
  
  // Initialise random NTL generator
  NTL::SetSeed(str2zz(seed));
  
  // Open file
  if (argc >= 2) {
    outfilename = argv[1];
  } else {
    std::cerr << argv[0] << ". WARNING: using default output file name." << std::endl;
  }
  outfile.open(outfilename, std::ofstream::out | std::ofstream::trunc);
  if (outfile.fail()) {
    std::cerr << argv[0] << ". Error opening file \"" << outfilename << "\"" << std::endl;
    exit(1);
  }
  
  outfile << n << "\n";
  // Generate stuff
  std::cout << "Generating numbers. This may take a while ...";
  std::cout.flush();
  for (size_t i = 0; i < n; i++) {
    outfile << NTL::RandomBnd(p) << "\n";
    if (i % (n / 10) == 0) {
      std::cout << ".";
      std::cout.flush();
    }
  }
  outfile << std::endl;
  std::cout << " done!" << std::endl;
  outfile.close();
  
  // Finish feedback
  std::cout << n << " pseudo-random numbers modulo " << p << " have been successfully written to file \"" << outfilename << "\"" << std::endl;
  
  return 0;
}
//-----------------------------------------------------------------------------
