#include <iostream>
#include <cstdlib>
#include <math.h>
#include <getopt.h>

#include "cherpars.h"
//-----------------------------------------------------------------------------

/**
 * Prints the usage of this executable
 * 
 * @param prgnam the name of the program/binary called
 */
static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " [-h] -l <size> -n <number> -p <probability> [-q]\n"
            << " -l : size of each bucket in the hash table\n"
            << " -n : 2^n numbers to store\n"
            << " -p : 2^p as the probability of overload\n"
            << " -q : be more quiet (just show the computed number of buckets)\n"
            << " -h : show this message and exit\n"
            << std::endl;
}
//-----------------------------------------------------------------------------

int main(int argc, char **argv) {
  double sigma, h;
  size_t l = DEFAULT_BUCKET_SIZE;
  size_t n = DEFAULT_N;
  double targetP = DEFAULT_TARGET_PROB_EXPONENT;
  double epsilon = 1e-4;
  double top, down, currentP;
  int quiet = 0;
  size_t mu;
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "hl:n:p:q")) != -1) {
    switch (op) {
      case 'l':
        if (atol(optarg) < 1L) {
          std::cerr << argv[0] << ". Fatal error: \"l\" must be at least 1." << std::endl;
          printUsage(argv[0]);
          exit(1);
        }
        l = atol(optarg);
        break;
        
      case 'n':
        if (atol(optarg) <= 0L) {
          std::cerr << argv[0] << ". Fatal error: \"n\" must be positive." << std::endl;
          exit(1);
        }
        n = atol(optarg);
        break;
        
      case 'p':
        if (atof(optarg) >= -epsilon) {
          std::cerr << argv[0] << ". Fatal error: \"p\" must be negative." << std::endl;
          printUsage(argv[0]);
          exit(1);
        }
        targetP = atof(optarg);
        break;
        
      case 'q':
        quiet = 1;
        break;
        
      case '?':
        std::cerr << argv[0] << "Unrecognised option " << argv[op] << std::endl;
        /* FALLTHRU */
      case 'h':
      default:
        printUsage(argv[0]);
        return 1;
    }
  }
  
  /*
   * Start from sigma = 0, increase to find the smallest sigma 
   * meets the targets
   */
  targetP = pow(2., targetP);
  n = pow(2., n);
  mu = l;
  do {
    sigma = (double) l / mu - 1.;
    h = ceil((double) n / mu);
    top = pow(exp(1.), sigma);
    down = pow(1. + sigma, 1. + sigma);
    currentP = h * pow(top / down, mu);
    mu--;
  } while (mu > 0 && currentP > targetP);
  
  // Print things out
  if (quiet) {
    std::cout << (size_t) floor(h) << std::endl;
  } else {
    std::cout << "Size of hash table = " << (size_t) floor(h) << std::endl;
    std::cout << "Size of bucket = " << l << std::endl;
    std::cout << "*************************" << std::endl;
    std::cout << "Overflow probability = " << currentP << std::endl;
    std::cout << "Target probability = " << targetP << std::endl;
    std::cout << "Mu = " << mu << std::endl;
    std::cout << "Sigma = " << sigma << std::endl;
    std::cout << "Efficiency factor = " << (h * l / n) << std::endl;
  }
  
  return 0;
}
//-----------------------------------------------------------------------------
