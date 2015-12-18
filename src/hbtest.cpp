#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <getopt.h>
#include <NTL/ZZ_p.h>

#include "hashbuckets.h"
#include "murmurhash3.h"
#include "shazzp.h"
#include "strint.h"
#include "simplebm.h"

#include "zzrndgen.h"
#include "hbtest.h"
//-----------------------------------------------------------------------------

static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -k <keys> -l <bucket-size> -i <infile>\n"
            << " -a : hash algorithm (MH3)\n"
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
  NTL::ZZ p;
  SimpleBenchmark benchmark;
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "a:hi:k:l:")) != -1) {
    switch (op) {
      case 'a':
        if (strcmp(optarg, "MH3") == 0) {
          hashAlgorithm = new MurmurHash3(DEFAULT_MURMURHASH_SEED);
        } else if (strcmp(optarg, "SHA1") == 0) {
          hashAlgorithm = new SHAZZp(SHA1_FLAVOUR);
        } else if (strcmp(optarg, "SHA256") == 0) {
          hashAlgorithm = new SHAZZp(SHA256_FLAVOUR);
        } else if (strcmp(optarg, "MD5") == 0) {
          std::cerr << argv[0] << ". Hash algorithm SHA1 not (yet) implemented." << std::endl;
          exit(2);
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
        
      case 'p': 
        p = str2zz(optarg);
        break;
        
      case '?':
        std::cerr << "Unrecognised option " << argv[op] << std::endl;
      case 'h':
      default:
        printUsage(argv[0]);
        return 1;
    }
  }
  
  // Use a specific seed to generate the same hashes
  if (hashAlgorithm == nullptr) {
    std::cerr << argv[0] << ". WARNING: using default hash algorithm MurmurHash3." << std::endl;
    hashAlgorithm = new MurmurHash3(DEFAULT_MURMURHASH_SEED);
  }
  hashBuckets = new HashBuckets<NTL::ZZ_p>(length, maxLoad);
  if (hashBuckets == nullptr) { // or is a try-catch more appropriate?
    std::cerr << argv[0] << ". Error allocating hash table." << std::endl;
    exit(1);
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
  
  // Initialize numbers modulo p
  infile >> p;
  NTL::ZZ_p::init(p);
  
  // Read all numbers at once (?)
  infile >> n;
  
  // Check arguments consistency
  if (n/length > maxLoad) {
    std::cerr << argv[0] << ". WARNING: too many numbers for the hashtable." << std::endl;
  }
    
  z = new NTL::ZZ_p[n];
  if (z == nullptr) {
    std::cerr << argv[0] << ". Error loading all " << n << " numbers to memory." << std::endl;
    exit(1);
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
  
  delete(hashAlgorithm);
  delete(hashBuckets);
  return 0;
}
//-----------------------------------------------------------------------------
