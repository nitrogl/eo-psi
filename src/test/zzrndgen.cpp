#include <iostream>
#include <cstdlib>
#include <fstream>
#include <getopt.h>

#include <libeopsi.h>
#include "zzrndgen.h"
//-----------------------------------------------------------------------------

/**
 * Prints the usage of this executable
 * 
 * @param prgnam the name of the program/binary called
 */
static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -h -n <number> -b <bits> -o <outfile>\n"
            << " -b : maximum number of bits for generated numbers\n"
            << " -n : amount of random numbers to generate\n"
            << " -o : file name to store generated numbers\n"
            << " -h : show this message and exit\n"
            << std::endl;
}
//-----------------------------------------------------------------------------

int main(int argc, char **argv) {
  NTL::ZZ supremum;
  std::ofstream outfile;
  size_t n = DEFAULT_N;
  size_t plainSetBits = DEFAULT_PLAIN_SET_BITS;
  std::string outfilename = DEFAULT_FILENAME;
  ZZPRF zzprf;
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "b:hn:o:")) != -1) {
    switch (op) {
      case 'b':
        if (atol(optarg) <=  0) {
          std::cerr << argv[0] << ". Number of bits for number generation must be positive." << std::endl;
          exit(1);
        }
        plainSetBits = atol(optarg);
        break;
        
      case 'n':
        n = atol(optarg);
        break;
        
      case 'o':
        outfilename = optarg;
        break;
        
      case '?':
        std::cerr << "Unrecognised option " << argv[op] << std::endl;
        /* FALLTHRU */
      case 'h':
      default:
        printUsage(argv[0]);
        return 1;
    }
  }
  // Initialise random seed
  srand(time(NULL));
  
  // Open file
  if (outfilename == DEFAULT_FILENAME) {
    std::cerr << argv[0] << ". WARNING: using default output file name." << std::endl;
  }
  outfile.open(outfilename, std::ofstream::out | std::ofstream::trunc);
  if (outfile.fail()) {
    std::cerr << argv[0] << ". Error opening file \"" << outfilename << "\"" << std::endl;
    exit(1);
  }
  
  outfile << plainSetBits << "\n";
  outfile << n << "\n";
  // Generate stuff
  std::cout << "Generating numbers. This may take a while ...";
  std::cout.flush();
  for (size_t i = 0; i < n; i++) {
    outfile << zzprf.generate(i, plainSetBits) << "\n";
    if (i % (n / 10) == 0) {
      std::cout << ".";
      std::cout.flush();
    }
  }
  outfile << std::endl;
  std::cout << " done!" << std::endl;
  outfile.close();
  
  // Finish feedback
  std::cout << n << " pseudo-random numbers of maximum " << plainSetBits << " have been successfully written to file \"" << outfilename << "\"" << std::endl;
  
  return 0;
}
//-----------------------------------------------------------------------------
