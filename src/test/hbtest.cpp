#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <getopt.h>
#include <NTL/ZZ_p.h>

#include <libhashbuckets.h>
#include "hbtest.h"
//-----------------------------------------------------------------------------

/**
 * Prints the usage of this executable
 * 
 * @param prgnam the name of the program/binary called
 */
static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -k <keys> -l <bucket-size> -i <infile>\n"
            << " -a : hash algorithm (MH3|SHA1|SHA256|SHA512)\n"
            << " -k : number of keys of the hash table\n"
            << " -l : size of buckets of the hash table\n"
            << " -i : file name to read numbers from\n"
            << " -h : show this message and exit\n"
            << std::endl;
}
//-----------------------------------------------------------------------------

/**
 * 1 - Arguments: file name, length (number of buckets) and max load for a bucket
 *     the file contains big integers modulo p
 * 2 - Create hash table
 *     a) hash each number and add to the hash table
 *     b) benchmark the insertion
 * 3 - output the size of buckets (uniformity is a must)
 */
int main(int argc, char **argv) {
  HashBuckets<NTL::ZZ_p>* hashBuckets = nullptr;
  HashAlgorithm<NTL::ZZ_p>* hashAlgorithm = nullptr;
  std::string infilename = DEFAULT_FILENAME;
  std::ifstream infile;
  size_t maxLoad = DEFAULT_HASHBUCKETS_MAXLOAD;
  size_t length = DEFAULT_HASHBUCKETS_LENGTH;
  size_t n;
  NTL::ZZ_p *z = nullptr;
  NTL::ZZ two;
  size_t bits;
  SimpleBenchmark benchmark;
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "a:hi:k:l:")) != -1) {
    switch (op) {
      case 'a':
        try {
          if (strcmp(optarg, "MH3") == 0) {
            hashAlgorithm = new MurmurHash3(DEFAULT_MURMURHASH_SEED);
          } else if (strcmp(optarg, "SHA1") == 0) {
            hashAlgorithm = new SHAZZp(SHA1_FLAVOUR);
          } else if (strcmp(optarg, "SHA256") == 0) {
            hashAlgorithm = new SHAZZp(SHA256_FLAVOUR);
          } else if (strcmp(optarg, "SHA512") == 0) {
            std::cerr << argv[0] << ". Hash algorithm SHA512 not (yet) implemented." << std::endl;
            exit(2);
          }
        } catch (std::bad_alloc &) {
          std::cerr << argv[0] << ". Error allocating memory." << std::endl;
          exit(1);
        }
        break;
        
      case 'k':
        length = atol(optarg); 
        break;
        
      case 'l':
        maxLoad = atol(optarg); 
        break;
        
      case 'i': 
        infilename = optarg; 
        break;
        
      case '?':
        std::cerr << "Unrecognised option " << argv[op] << std::endl;
      case 'h':
      default:
        printUsage(argv[0]);
        return 1;
    }
  }
  
  try {
    // Use a specific seed to generate the same hashes
    if (hashAlgorithm == nullptr) {
      std::cerr << argv[0] << ". WARNING: using default hash algorithm MurmurHash3." << std::endl;
      hashAlgorithm = new MurmurHash3(DEFAULT_MURMURHASH_SEED);
    }
    hashBuckets = new HashBuckets<NTL::ZZ_p>(length, maxLoad);
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error allocating hash table." << std::endl;
    exit(2);
  }
  hashBuckets->setHashAlgorithm(hashAlgorithm);
  
  // Open file with numbers
  if (infilename == DEFAULT_FILENAME) {
    std::cerr << argv[0] << ". WARNING: using default input file name \"" << infilename << "\"." << std::endl;
  }
  infile.open(infilename, std::ifstream::in);
  if (infile.fail()) {
    std::cerr << argv[0] << ". Error opening file \"" << infilename << "\"" << std::endl;
    exit(1);
  }
  
  // Initialise numbers modulo p
  infile >> bits;
  conv(two, 2);
  NTL::ZZ_p::init(NTL::power(two, bits));
  
  // Read the number of numbers
  infile >> n;
  
  // Check arguments consistency
  if (n/length > maxLoad) {
    std::cerr << argv[0] << ". WARNING: too many numbers for the hashtable." << std::endl;
  }
  
  try {
    z = new NTL::ZZ_p[n];
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error allocating space for the " << n << " numbers to memory." << std::endl;
    exit(2);
  }
  for (size_t i = 0; i < n; i++) {
    infile >> z[i];
  }
  
  // Add to hash
  benchmark.start();
  for (size_t i = 0; i < n; i++) {
    hashBuckets->add(z[i]);
  }
  benchmark.stop();
  
  // Print stats
  hashBuckets->printStats();
  std::cout << "Total time to add " << n << " elements to the hashtable: " << benchmark.benchmark().count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to add an element to the hashtable: " << (double) benchmark.benchmark().count() / n << " µs" << std::endl;
  
  delete [] z;
  delete hashAlgorithm;
  delete hashBuckets;
  return 0;
}
//-----------------------------------------------------------------------------
