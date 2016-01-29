#include <iostream>
#include <cstdlib>
#include <fstream>
#include <getopt.h>

#include <libhashbuckets.h>
#include "eopsisimul.h"
//-----------------------------------------------------------------------------

static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -h -p <modulo> -n <number> -o <outfile>\n"
            << " -p : set p generating numbers modulo p (big integer)\n"
            << " -n : amount of random numbers to generate\n"
            << " -o : file name to store generated numbers\n"
            << " -h : show this message and exit\n"
            << std::endl;
}
//-----------------------------------------------------------------------------

int main(int argc, char **argv) {
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "hn:o:p:")) != -1) {
    switch (op) {
      case 'n':
        break;
        
      case 'o':
        break;
        
      case 'p':
        break;
        
      case '?':
        std::cerr << "Unrecognised option " << argv[op] << std::endl;
      case 'h':
      default:
        printUsage(argv[0]);
        return 1;
    }
  }
  // Initialize random seed
  srand(time(NULL));
  
  return 0;
}
//-----------------------------------------------------------------------------
