#include <iostream>
#include <fstream>
#include <gmpxx.h>
#include <getopt.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>

#include <libhashbuckets.h>
#include "bptest.h"
//-----------------------------------------------------------------------------

static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -h -k <keys> -l <bucket-size> -p <pad-to-bits> -i <infile>\n"
            << " -a : hash algorithm (MH3|SHA1|SHA256|SHA512)\n"
            << " -k : number of keys of the hash table\n"
            << " -l : size of buckets of the hash table\n"
            << " -p : pad numbers up to a specific number of bits\n"
            << " -i : file name to read numbers from\n"
            << " -h : show this message and exit\n"
            << std::endl;
}
//-----------------------------------------------------------------------------

/*
 * 
 */
int main(int argc, char **argv) {
  HashBuckets<NTL::ZZ_p>* hashBuckets = nullptr;
  HashAlgorithm<NTL::ZZ_p>* hashAlgorithm = nullptr;
  std::string infilename = DEFAULT_FILENAME;
  std::ifstream infile;
  size_t maxLoad = DEFAULT_HASHBUCKETS_MAXLOAD;
  size_t length = DEFAULT_HASHBUCKETS_LENGTH;
  size_t n;
  double progress;
  unsigned int padsize = DEFAULT_PADSIZE;
  unsigned int bs, topad;
  RandomZZpGenerator rndZZpgen;
  NTL::ZZ_p *z = nullptr;
  NTL::ZZ p, tmpZ;
  NTL::ZZ_pX *polynomials = nullptr;
  NTL::ZZ_p *incognita = nullptr;
  NTL::ZZ **evaluations;
  NTL::Vec<NTL::ZZ_p> vzzp;
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "a:hi:k:l:p:")) != -1) {
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
        
      case 'p': 
        padsize = atoi(optarg);
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
    polynomials = new NTL::ZZ_pX[length];
    incognita = new NTL::ZZ_p[2*maxLoad + 1];
    evaluations = new NTL::ZZ*[length];
    for (size_t i = 0; i < length; i++) {
      evaluations[i] = new NTL::ZZ[2*maxLoad + 1];
    }
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error allocating memory." << std::endl;
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
  
  // Initialize numbers modulo p
  infile >> p;
  bs = NTL::bits(p);
  p = p << (padsize > bs ? padsize - bs : 0L);
  NTL::ZZ_p::init(p);
  
  // Read the number of numbers
  infile >> n;
  
  // Check arguments consistency
  if (n/length > maxLoad) {
    std::cerr << argv[0] << ". WARNING: too many numbers for the hashtable." << std::endl;
  }
  
  std::cout << "Reading and padding numbers... ";
  std::cout.flush();
  try {
    z = new NTL::ZZ_p[n];
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error loading all " << n << " numbers to memory." << std::endl;
    exit(2);
  }
  for (size_t i = 0; i < n; i++) {
    infile >> tmpZ;
    bs = NTL::bits(tmpZ);
    topad = (padsize > bs ? padsize - bs : 0L);
    if (topad > 0L) {
//       std::cout << NTL::bits(tmpZ) << " -> ";
      tmpZ = tmpZ << topad - 1;
//       std::cout << NTL::bits(z[i]) << std::endl;
    } else {
      conv(z[i], tmpZ);
    }
    conv(z[i], tmpZ);
  }
  std::cout << "done. " << std::endl;
  
  // Add to hash
  std::cout << "Populating hash table... ";
  std::cout.flush();
  for (size_t i = 0; i < n; i++) {
    hashBuckets->add(z[i]);
  }
  std::cout << "done. " << std::endl;
  
  // Fill empty cells of the hash table
  std::cout << "Concealing the hash table (filling empty cells)... ";
  std::cout.flush();
  rndZZpgen.setModulo(p);
  hashBuckets->conceal(rndZZpgen);
  std::cout << "done. " << std::endl;
  
  // Creating polynomials
  std::cout << "Generating polynomials... ";
  std::cout.flush();
  for (size_t i = 0; i < length; i++) {
    polynomials[i] = NTL::BuildFromRoots(NTL::vector2VecZZp(*(hashBuckets->getBucket(i))));
  }
  std::cout << "done. " << std::endl;
  
  delete(hashAlgorithm);
  delete(hashBuckets);
  return 0;
}
//-----------------------------------------------------------------------------
