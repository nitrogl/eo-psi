#include <iostream>
#include <fstream>
#include <thread>
#include <gmpxx.h>
#include <getopt.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>

#include <libeopsi.h>
#include "bptest.h"
//-----------------------------------------------------------------------------

/**
 * Prints the usage of this executable
 * 
 * @param prgnam the name of the program/binary called
 */
static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -h -k <buckets> -l <bucket-size> -p <field-size> -i <infile>\n"
            << " -a : hash algorithm (MH3|SHA1|SHA256|SHA512)\n"
//             << " -j : force number of threads for evaluation (be sure your NTL is thread-safe)\n"
            << " -k : number of buckets of the hash table\n"
            << " -l : size of each bucket in the hash table\n"
            << " -p : set the field size bits to p\n"
            << " -i : file name to read numbers from\n"
            << " -h : show this message and exit\n"
            << std::endl;
}
//-----------------------------------------------------------------------------

/**
 * The function to call from each thread to evaluate polynomials
 * 
 * @param evaluations the result is stored here
 * @param polynomials the polynomials to evaluate
 * @param unknowns the unknowns to use in the evaluations
 * @param from the first index for the subset of unknowns to evaluate
 * @param from the last index (excluded) for the subset of unknowns to evaluate
 */
static void polEval(NTL::vec_ZZ_p *evaluations, NTL::ZZ_pX *polynomials, NTL::vec_ZZ_p unknowns, const size_t from, const size_t to) {
  size_t j;
  for (j = from; j < to; j++) {
    eval(evaluations[j], polynomials[j], unknowns);
  }
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
  size_t n, index;
  size_t pbits = DEFAULT_FIELD_SIZE_BITS;
  unsigned int padsize;
  ZZpPRF *zzpprf;
  NTL::ZZ_p *zzpSeed = nullptr;
  NTL::ZZ_p *z = nullptr;
  NTL::ZZ p, tmpZ;
  NTL::ZZ_pX *polynomials = nullptr;
  NTL::vec_ZZ_p unknowns;
  NTL::vec_ZZ_p *evaluations;
  NTL::vec_ZZ_p vzzp;
  HashAlgorithm<std::string>* strHashAlgorithm = nullptr;
  SimpleBenchmark benchmark;
  unsigned int cores = 0, nThreads = 0;
  size_t nSplit;
  size_t *cumulSplit;
  std::thread *threads;
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "a:hi:j:k:l:p:")) != -1) {
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
        
      case 'j':
        if (atol(optarg) < 0) {
          std::cerr << argv[0] << ". Number of threads can't be negative." << std::endl;
          exit(1);
        }
        nThreads = atol(optarg); 
        break;
        
      case 'k':
        if (atol(optarg) <= 0) {
          std::cerr << argv[0] << ". Number of buckets must be positive." << std::endl;
          exit(1);
        }
        length = atol(optarg); 
        break;
        
      case 'l':
        if (atol(optarg) <= 0) {
          std::cerr << argv[0] << ". Size of buckets must be positive." << std::endl;
          exit(1);
        }
        maxLoad = atol(optarg); 
        break;
        
      case 'i': 
        infilename = optarg; 
        break;
        
      case 'p': 
        if (atol(optarg) <= 0) {
          std::cerr << argv[0] << ". Number of field size bits must be greater than zero." << std::endl;
          exit(1);
        }
        pbits = atol(optarg);
        break;
        
      case '?':
        std::cerr << argv[0] << ". Unrecognised option " << argv[op] << std::endl;
        /* FALLTHRU */
      case 'h':
      default:
        printUsage(argv[0]);
        return 1;
    }
  }
  
  // Initialise numbers modulo p
  NTL::SetSeed(NTL::to_ZZ(time(NULL)));
  p = NTL::GenPrime_ZZ(pbits, 100);
  if (!NTL::ProbPrime(p)) {
    std::cerr << argv[0] << ". " << p << " does not look like a prime number. Aborting..." << std::endl;
    exit(1);
  }
  NTL::ZZ_p::init(p);
  padsize = NTL::NumBits(p);
  
  // Number of threads
  if (nThreads == 0) {
#ifndef NTL_THREADS_BOOST
    // Automatic number of threads if boost is not available
    cores = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1;
    nThreads = cores;
#endif
    nThreads = 1; // [[TODO]] automatically is single-threaded
  }
  
  // To enable threads you need thread NTL to be compiled enabling thread-safe
#ifndef NTL_THREADS
  std::cerr << argv[0] << ". Your NTL library is not thread-safe: using only 1 thread." << std::endl;
  nThreads = 1;
#endif
  if (nThreads > 1) {
    std::cerr << argv[0] << ". Multithreading not (yet) implemented." << std::endl;
  }
  nThreads = 1;
  
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
    hashBuckets = new HashBuckets<NTL::ZZ_p>(length, maxLoad, hashAlgorithm);
    polynomials = new NTL::ZZ_pX[length];
    evaluations = new NTL::vec_ZZ_p[length];
    strHashAlgorithm = new SHAString(SHA1_FLAVOUR);
    zzpprf = new ZZpPRF(p);
    threads = new std::thread[nThreads];
    cumulSplit = new size_t[nThreads + 1];
    zzpSeed = new NTL::ZZ_p();
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error allocating memory." << std::endl;
    exit(2);
  }
  
  // Ignore the original boundary of creation
  infile >> tmpZ;
  
  // Read the number of numbers (The same line as before...)
  infile >> n;
  
  // Check arguments consistency
  if (n/length > maxLoad) {
    std::cerr << argv[0] << ". WARNING: too many numbers for the hashtable." << std::endl;
  }
  
  std::cout << "Reading " << n << " " << NTL::NumBits(tmpZ) << "-bit numbers and padding to " << padsize << " bits... ";
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
    conv(z[i], NTL::zeroPad(tmpZ, padsize));
  }
  benchmark.step("padding");
  std::cout << "done. " << std::endl;
  
  // Add to hash
  std::cout << "Populating hash table... ";
  std::cout.flush();
  benchmark.step();
  for (size_t i = 0; i < n; i++) {
    hashBuckets->add(z[i]);
  }
  benchmark.step("hashbuckets");
  std::cout << "done. " << std::endl;
  
  // Fill empty cells of the hash table
  std::cout << "Concealing the hash table (filling empty cells)... ";
  std::cout.flush();
  conv(*zzpSeed, 0);
  benchmark.step();
  hashBuckets->conceal(*zzpprf);
  benchmark.step("conceal");
  std::cout << "done. " << std::endl;
  
  // Creating polynomials
  std::cout << "Generating polynomials... ";
  std::cout.flush();
  benchmark.step();
  for (size_t i = 0; i < length; i++) {
    polynomials[i] = NTL::BuildFromRoots(NTL::vector2VecZZp(hashBuckets->getBucket(i)));
  }
  benchmark.step("polgen");
  std::cout << "done. " << std::endl;
  
  // Generating random unknowns
  std::cout << "Generating random unknowns... ";
  std::cout.flush();
  unknowns.SetLength(2*maxLoad + 1);
  benchmark.step();
  for (size_t j = 0; j < 2*maxLoad + 1; j++) {
    append(unknowns, zzpprf->generate(*zzpSeed, j));
  }
  benchmark.step("unkgen");
  std::cout << "done. " << std::endl;
  
  // Evaluating polynomials
  std::cout << "Evaluating polynomials... ";
  std::cout.flush();
  benchmark.step();
  if (nThreads > 1) {
    // Also the preparation bust be benchmarked
    // Compute the loop boundaries
    nSplit = (length + 1) / nThreads;
    cumulSplit[0] = 0;
    for (size_t i = 1; i <= nThreads; i++) {
      cumulSplit[i] = cumulSplit[i - 1] + nSplit;
    }
    // Do not exceed
    if (cumulSplit[nThreads] > length) {
      cumulSplit[nThreads] = length;
    }
    
    std::cout << "(" << cumulSplit[0];
    for (size_t i = 1; i <= nThreads; i++) {
      std::cout << "-" << cumulSplit[i];
    }
    std::cout << ")... ";
    std::cout.flush();
    
    // Run threads
    for (size_t i = 0; i < nThreads - 1; i++) {
      threads[i] = std::thread(polEval, evaluations, polynomials, unknowns, cumulSplit[i], cumulSplit[i + 1]);
    }
    
    // Do something in this thread too
    polEval(evaluations, polynomials, unknowns, cumulSplit[nThreads - 1], cumulSplit[nThreads]);
    
    // Wait for all threads
    for (size_t i = 0; i < nThreads - 1; i++) {
      threads[i].join();
    }
  } else {
    // Evaluating polynomials (single core)
    for (size_t j = 0; j < length; j++) {
      eval(evaluations[j], polynomials[j], unknowns);
    }
  }
  benchmark.step("poleval");
  std::cout << "done. " << std::endl;
  
  // Blind evaluations
  std::cout << "Blinding evaluations... ";
  std::cout.flush();
  benchmark.step();
  conv(*zzpSeed, NTL::ZZFromBytes((byte *) "Topsy Kretts", 12));
  index = 0;
  for (size_t j = 0; j < length; j++) {
    for (size_t i = 0; i < 2*maxLoad + 1; i++) {
      evaluations[j][i] = evaluations[j][i] + zzpprf->generate(*zzpSeed, index++);
    }
  }
  benchmark.stop("blind");
  std::cout << "done. " << std::endl;
  
  // Stats
  std::cout << std::endl;
  std::cout << "Total time to read and pad " << n << " numbers: "
            << benchmark.benchmark("padding").count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to read and pad a number: "
            << (double) benchmark.benchmark("padding").count() / n << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to add " << n << " elements to the hashtable: " 
            << benchmark.benchmark("hashbuckets").count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to add an element to the hashtable: " 
            << (double) benchmark.benchmark("hashbuckets").count() / n << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to conceal the hashtable (fill empty buckets): " 
            << benchmark.benchmark("conceal").count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to fill each bucket: " 
            << (double) benchmark.benchmark("conceal").count() / length << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to generate " << length << " polynomials: " 
            << benchmark.benchmark("polgen").count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to generate each polynomial: " 
            << (double) benchmark.benchmark("polgen").count() / length << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to generate " << (2*maxLoad + 1) << " random unknowns: " 
            << benchmark.benchmark("unkgen").count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to generate each unknown: " 
            << (double) benchmark.benchmark("unkgen").count() / (2*maxLoad + 1) << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to evaluate " << length << " polynomials against " << (2*maxLoad + 1) << " unknowns: " 
            << benchmark.benchmark("poleval").count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to generate each evaluation: " 
            << (double) benchmark.benchmark("poleval").count() / (length * (2*maxLoad + 1)) * nThreads << " µs" << std::endl;
  std::cout << std::endl;
            
  std::cout << "Total time to blind " << length << "x" << (2*maxLoad + 1) << " evaluations: " 
            << benchmark.benchmark("blind").count() / 1000000. << " s" << std::endl; 
  std::cout << "Average time to blind each evaluation: " 
            << (double) benchmark.benchmark("blind").count() / (length * (2*maxLoad + 1)) << " µs" << std::endl;
  std::cout << std::endl;
  
  delete [] z;
  delete hashAlgorithm;
  delete hashBuckets;
  delete cumulSplit;
  delete strHashAlgorithm;
  delete zzpprf;
  return 0;
}
//-----------------------------------------------------------------------------
