#include <iostream>
#include <cstdlib>
#include <fstream>
#include <getopt.h>

#include <NTL/ZZ_pXFactoring.h>
#include <NTL/vec_ZZ_p.h>
#include <NTL/pair_ZZ_pX_long.h>
#include <libeopsi.h>
#include "polyfact.h"
//-----------------------------------------------------------------------------

/**
 * Prints the usage of this executable
 * 
 * @param prgnam the name of the program/binary called
 */
static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -h -q -b <bits> -d <number> -n <number>\n"
            << " -b : maximum number of bits for generated roots\n"
            << " -d : degree of polynomials\n"
            << " -n : number of experiments\n"
            << " -q : be quiet (shows only factorisation average, error, min and max in ms, and total in seconds)\n"
            << " -h : show this message and exit\n"
            << std::endl;
}
//-----------------------------------------------------------------------------

int main(int argc, char **argv) {
  NTL::ZZ_pX *polynomials;
  NTL::vec_ZZ_p roots;
  SimpleBenchmark *bmGen, *bmFact;
  size_t degree = DEFAULT_DEGREE;
  size_t bits = DEFAULT_BITS;
  size_t n = DEFAULT_N;
  size_t k;
  bool quiet = DEFAULT_QUIET;
  NTL::ZZ seed, modulo;
  NTL::ZZ_p seedP;
  ZZpPRF *zzpprf;
  NTL::vec_pair_ZZ_pX_long factResult;
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "b:d:hn:o:q")) != -1) {
    switch (op) {
      case 'b':
        if (atol(optarg) <=  0) {
          std::cerr << argv[0] << ". Number of bits for number generation must be positive." << std::endl;
          exit(1);
        }
        bits = atol(optarg);
        break;
        
      case 'd':
        if (atol(optarg) <  0) {
          std::cerr << argv[0] << ". Degree of polynomials can not be negative." << std::endl;
          exit(1);
        }
        degree = atol(optarg);
        break;
        
      case 'n':
        if (atol(optarg) <=  0) {
          std::cerr << argv[0] << ". Number of experiments must be at least 1." << std::endl;
          exit(1);
        }
        n = atol(optarg);
        break;
        
      case 'q':
        quiet = true;
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
  
  // Initialise modulo operations in NTL
  seed = NTL::to_ZZ(time(NULL));
  NTL::SetSeed(seed);
  modulo = NTL::GenPrime_ZZ(bits, 100);
  if (!NTL::ProbPrime(modulo)) {
    std::cerr << argv[0] << ". " << modulo << " does not look like a prime number. Aborting..." << std::endl;
    exit(1);
  }
  NTL::ZZ_p::init(modulo);
  conv(seedP, seed);
  
  try {
    zzpprf = new ZZpPRF(modulo);
    polynomials = new NTL::ZZ_pX[n];
    bmGen = new SimpleBenchmark(n + 2);
    bmFact = new SimpleBenchmark(n + 2);
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error allocating memory." << std::endl;
    exit(2);
  }
  
  // Data feedback
  if (!quiet) {
    std::cout << " -- Data"
              << "\n\tRandom prime modulo = " << modulo << " (" << NTL::NumBits(modulo) << " bits)."
              << "\n\tNumber of experiments = " << n
              << "\n\tDegree = " << degree
              << "\n" << std::endl;
    std::cout << " -- Running experiments...";
    std::cout.flush();
  }
  
  // Generate random polynomials
  roots.SetLength(degree);
  k = 0;
  for (size_t j = 0; j < n; j++) {
    bmGen->step();
    for (size_t i = 0; i < degree; i++) {
      roots[i] = zzpprf->generate(seedP, k++);
    }
    polynomials[j] = NTL::BuildFromRoots(roots);
    bmGen->pause();
  
    // Factorise
    bmFact->step();
    NTL::CanZass(factResult, polynomials[j]);
    bmFact->pause();
    
    if (!quiet && !(j % (n/10))) {
      std::cout << ".";
      std::cout.flush();
    }
  }
  bmGen->stop();
  bmFact->stop();
  
  if (!quiet) {
    std::cout << " done.\n" << std::endl;
  
    // Stats
    std::cout << " -- Random Generation"
              << "\n\t   Avg: " << bmGen->average()/1000. << " ms"
              << "\n\tStdDev: " << bmGen->standardDeviation()/1000. << " ms"
              << "\n\t   Min: " << bmGen->min().count()/1000. << " ms"
              << "\n\t   Max: " << bmGen->max().count()/1000. << " ms"
              << "\n\t Total: " << bmGen->cumulativeBenchmark().count()/1000000. << " s"
              << "\n" << std::endl;
    std::cout << " -- Factorisation"
              << "\n\t   Avg: " << bmFact->average()/1000. << " ms"
              << "\n\tStdDev: " << bmFact->standardDeviation()/1000. << " ms"
              << "\n\t   Min: " << bmFact->min().count()/1000. << " ms"
              << "\n\t   Max: " << bmFact->max().count()/1000. << " ms"
              << "\n\t Total: " << bmFact->cumulativeBenchmark().count()/1000000. << " s"
              << "\n" << std::endl;
  } else {
    std::cout << bmFact->average()/1000. << "\t"
              << bmFact->standardDeviation()/1000. << "\t"
              << bmFact->min().count()/1000. << "\t"
              << bmFact->max().count()/1000. << "\t"
              << bmFact->cumulativeBenchmark().count()/1000000.
              << std::endl;
  }
  
  delete [] polynomials;
  delete zzpprf;
  delete bmFact;
  delete bmGen;
  
  return 0;
}
//-----------------------------------------------------------------------------
