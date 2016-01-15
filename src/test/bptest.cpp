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
  unsigned int padsize = DEFAULT_PADSIZE;
  unsigned int bs, topad;
  RandomZZpGenerator *rndZZpgen;
  NTL::ZZ_p *z = nullptr;
  NTL::ZZ p, tmpZ;
  NTL::ZZ_pX *polynomials = nullptr;
  NTL::vec_ZZ_p unknowns;
  NTL::vec_ZZ_p *evaluations;
  NTL::vec_ZZ_p vzzp;
  HashAlgorithm<std::string>* strHashAlgorithm = nullptr;
  StringZZpKeyGenerator keygen;
  RandomZZpGenerator *prf;
  SimpleBenchmark benchmark;
  
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
  
  
  // Open file with numbers
  if (infilename == DEFAULT_FILENAME) {
    std::cerr << argv[0] << ". WARNING: using default input file name \"" << infilename << "\"." << std::endl;
  }
  infile.open(infilename, std::ifstream::in);
  if (infile.fail()) {
    std::cerr << argv[0] << ". Error opening file \"" << infilename << "\"" << std::endl;
    exit(1);
  }
  
  try {
    // Use a specific seed to generate the same hashes
    if (hashAlgorithm == nullptr) {
      std::cerr << argv[0] << ". WARNING: using default hash algorithm MurmurHash3." << std::endl;
      hashAlgorithm = new MurmurHash3(DEFAULT_MURMURHASH_SEED);
    }
    hashBuckets = new HashBuckets<NTL::ZZ_p>(length, maxLoad);
    polynomials = new NTL::ZZ_pX[length];
    evaluations = new NTL::vec_ZZ_p[length];
    strHashAlgorithm = new SHAString(SHA1_FLAVOUR);
    prf = rndZZpgen = new RandomZZpGenerator();
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error allocating memory." << std::endl;
    exit(2);
  }
  hashBuckets->setHashAlgorithm(hashAlgorithm);
  
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
  benchmark.start();
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
  benchmark.step();
  std::cout << "done. " << std::endl;
  
  // Add to hash
  std::cout << "Populating hash table... ";
  std::cout.flush();
  benchmark.step();
  for (size_t i = 0; i < n; i++) {
    hashBuckets->add(z[i]);
  }
  benchmark.step();
  std::cout << "done. " << std::endl;
  
  // Fill empty cells of the hash table
  std::cout << "Concealing the hash table (filling empty cells)... ";
  std::cout.flush();
  rndZZpgen->setModulo(p);
  benchmark.step();
  hashBuckets->conceal(*rndZZpgen);
  benchmark.step();
  std::cout << "done. " << std::endl;
  
  // Creating polynomials
  std::cout << "Generating polynomials... ";
  std::cout.flush();
  benchmark.step();
  for (size_t i = 0; i < length; i++) {
    polynomials[i] = NTL::BuildFromRoots(NTL::vector2VecZZp(*(hashBuckets->getBucket(i))));
  }
  benchmark.step();
  std::cout << "done. " << std::endl;
  
  // Generating random unknowns
  std::cout << "Generating random unknowns... ";
  std::cout.flush();
  unknowns.SetLength(2*maxLoad + 1);
  benchmark.step();
  for (size_t j = 0; j < 2*maxLoad + 1; j++) {
    append(unknowns, rndZZpgen->next());
  }
  benchmark.step();
  std::cout << "done. " << std::endl;
  
  // Evaluating polynomials
  std::cout << "Evaluating polynomials... ";
  std::cout.flush();
  benchmark.step();
  for (size_t i = 0; i < length; i++) {
    eval(evaluations[i], polynomials[i], unknowns);
  }
  benchmark.step();
  std::cout << "done. " << std::endl;
  
  // Initialise key generator and pseudo-random function
  keygen.setHashAlgorithm(strHashAlgorithm);
  keygen.setSecretKey("Topsy Kretts");
  keygen.setModulo(p);
  prf->setModulo(p);
  
  // Blind evaluations
  std::cout << "Blinding evaluations... ";
  std::cout.flush();
  benchmark.step();
  for (size_t i = 0; i < length; i++) {
    prf->setSeed(keygen.next());
    for (size_t j = 0; j < 2*maxLoad + 1; j++) {
      evaluations[i][j] = evaluations[i][j] + prf->next();
    }
  }
  benchmark.stop();
  std::cout << "done. " << std::endl;
  
  // Stats
  std::cout << std::endl;
  std::cout << "Total time to read and pad " << n << " numbers: "
            << benchmark.benchmark(1).count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to read and pad a number: "
            << (double) benchmark.benchmark(1).count() / n << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to add " << n << " elements to the hashtable: " 
            << benchmark.benchmark(3).count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to add an element to the hashtable: " 
            << (double) benchmark.benchmark(3).count() / n << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to conceal the hashtable (fill empty buckets): " 
            << benchmark.benchmark(5).count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to fill each bucket: " 
            << (double) benchmark.benchmark(5).count() / length << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to generate " << length << " polynomials: " 
            << benchmark.benchmark(7).count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to generate each polynomial: " 
            << (double) benchmark.benchmark(7).count() / length << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to generate " << (2*maxLoad + 1) << " random unknowns: " 
            << benchmark.benchmark(9).count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to generate each unknown: " 
            << (double) benchmark.benchmark(9).count() / (2*maxLoad + 1) << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to evaluate " << length << " polynomials against " << (2*maxLoad + 1) << " unknowns: " 
            << benchmark.benchmark(11).count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to generate each evaluation: " 
            << (double) benchmark.benchmark(11).count() / (length * (2*maxLoad + 1)) << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to blind " << length << "x" << (2*maxLoad + 1) << " evaluations: " 
            << benchmark.benchmark(13).count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to blind each evaluation: " 
            << (double) benchmark.benchmark(13).count() / (length * (2*maxLoad + 1)) << " µs" << std::endl;
  std::cout << std::endl;
  
  delete(hashAlgorithm);
  delete(hashBuckets);
  return 0;
}
//-----------------------------------------------------------------------------
