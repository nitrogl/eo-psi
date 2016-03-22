#include <iostream>
#include <cstdlib>
#include <fstream>
#include <getopt.h>
#include <cstdarg>
#include <set>

#include <libhashbuckets.h>
#include "eopsisimul.h"
//-----------------------------------------------------------------------------

/**
 * Prints the usage of this executable
 * 
 * @param prgnam the name of the program/binary called
 */
static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -h -b <bits-in-plain-sets> -p <field-size-bits> -k <buckets> -l <bucket-size>-n <number> -r <min-common>\n"
//             << " -j : force number of threads for evaluation (be sure your NTL is thread-safe)\n"
            << " -k : number of buckets of the hash table\n"
            << " -l : size of each bucket in the hash table\n"
            << " -p : set the field size bits to p\n"
            << " -b : number of bits in plain sets\n"
            << " -n : amount of random numbers to generate\n"
            << " -r : amount of minimum common data for clients (less or equal to n)\n"
            << " -h : show this message and exit\n"
            << std::endl;
}
//-----------------------------------------------------------------------------

/**
 * This is to authenticate parties each other.
 * In the lucky case it requires O(n²), where n is the arity.
 */
template <typename... Args>
static void authentication(Args... args) {
  EOPSIParty *parties[] = { args... };
  size_t i, j;
  
  // Authenticate parties each other
  for (i = 0; i < sizeof...(args); i++) {
    for (j = 0; j < sizeof...(args); j++) {
      parties[i]->authenticate(*parties[j]);
    }
  }
}
//-----------------------------------------------------------------------------

/**
 * This function join two sets regardless of eventual duplicates.
 * 
 * @param set1 the first set
 * @param card1 the cardinality of the first set
 * @param set2 the second set
 * @param card2 the cardinality of the second set
 * @return A pointer to the bare union which should be manually deleted.
 */
static NTL::ZZ* join(const NTL::ZZ *set1, const size_t card1, const NTL::ZZ *set2, const size_t card2) {
  NTL::ZZ *res;
  size_t i;
  
  try {
    res = new NTL::ZZ[card1 + card2];
  } catch (std::bad_alloc &) {
    std::cerr << "join(). Error allocating memory." << std::endl;
    exit(1);
  }
  
  // Copy first set
  for (i = 0; i < card1; i++) {
    res[i] = set1[i];
  }
  
  // Copy second set
  for (i = 0; i < card2; i++) {
    res[card1 + i] = set2[i];
  }
  
  return res;
}
//-----------------------------------------------------------------------------

/**
 * This function computes the intersection of two sets with no duplicates.
 * WARNING: the original sets must not have any duplicate.
 * 
 * @param set1 the first set
 * @param card1 the cardinality of the first set
 * @param set2 the second set
 * @param card2 the cardinality of the second set
 * @param card pointer to the cardinality of the intersection to save
 * @return A pointer to the intersection which should be manually deleted.
 */
static NTL::ZZ* intersect(const NTL::ZZ *set1, const size_t card1, const NTL::ZZ *set2, const size_t card2, size_t *card) {
  NTL::ZZ *res, *tmp;
  NTL::ZZ *listUnion;
  size_t i, k;
  auto compare = [](const void *a, const void *b) {
    NTL::ZZ n1 = (*(NTL::ZZ *) a);
    NTL::ZZ n2 = (*(NTL::ZZ *) b);
    return n1 == n2 ? 0 : (n1 < n2 ? -1 : 1);
  };
  
  // Join and sort them
  listUnion = join(set1, card1, set2, card2);
  qsort(listUnion, card1 + card2, sizeof(NTL::ZZ), compare);
  
  // One loop to find them...
  *card = 0;
  tmp = nullptr;
  for (i = 1; i < card1 + card2; i++) {
    if (listUnion[i] == listUnion[i - 1] && (tmp == nullptr || listUnion[i] != *tmp)) {
      (*card)++;
      tmp = &listUnion[i];
    }
  }
  
  // ... and in the darkness bind them!
  try {
    res = new NTL::ZZ[*card];
  } catch (std::bad_alloc &) {
    std::cerr << "intersect(). Error allocating memory." << std::endl;
    exit(1);
  }
  k = 0;
  for (i = 1; k < *card; i++) {
    if (listUnion[i] == listUnion[i - 1] && (k == 0 || listUnion[i] != res[k - 1])) {
      res[k++] = listUnion[i];
    }
  }
  
  delete [] listUnion;
  return res;
}
//-----------------------------------------------------------------------------

bool checkIntersectionCorrectness(const NTL::ZZ *int1, const size_t n, const NTL::vec_ZZ int2) {
  std::set<NTL::ZZ> set1;
  std::set<NTL::ZZ> set2;
  
  if (n != (size_t) int2.length()) {
    std::cerr << "checkIntersectionCorrectness(). Different intersections' size." << std::endl;
    return false;
  }
  
  for (size_t i = 0; i < n; i++) {
    set1.insert(int1[i]);
    set2.insert(int2[i]);
  }
  
  return set1 == set2;
}
//-----------------------------------------------------------------------------

/**
 * This function generates random ZZ numbers which are data stored in to 
 * the cloud.
 * 
 * @param n the amount of data to generate
 * @param bitsize maximum bit size of generation
 * @return A pointer to the data which should be manually deleted.
 */
NTL::ZZ * randomData(const size_t n, size_t bitsize) {
  NTL::ZZ *data;
  std::set<NTL::ZZ> dataSet;
  std::set<NTL::ZZ>::iterator elem;
  size_t i;
  
  if (n == 0) {
    std::cerr << "randomData(). No elements to create (" << n << ")." << std::endl;
    return nullptr;
  }
  
  try {
    data = new NTL::ZZ[n];
  } catch (std::bad_alloc &) {
    std::cerr << "randomData(). Error allocating memory." << std::endl;
    exit(1);
  }
  
  do {
    dataSet.insert(NTL::RandomBits_ZZ(bitsize));
  } while(dataSet.size() < n);
  
  i = 0;
  for (elem = dataSet.begin(); elem != dataSet.end(); ++elem) {
    data[i++] = *elem;
  }
  
  return data;
}
//-----------------------------------------------------------------------------

int main(int argc, char **argv) {
  EOPSIClient *alice, *bob;
  EOPSIServer *cloud;
  EOPSIMessage msgStoreDataAlice, msgStoreDataBob;
  EOPSIMessage msgBobAlice, msgAliceBob, msgAliceCloud, msgCloudBob;
  NTL::ZZ fieldsize;
  NTL::ZZ *dataAlice, *dataBob, *minCommonData, *uncommonDataA, *uncommonDataB;
  NTL::ZZ *setcap;
  size_t setcapCard;
  size_t plainSetBits = DEFAULT_PLAIN_SET_BITS;
  size_t fieldsizeBits = DEFAULT_FIELD_SIZE_BITS;
  size_t n = DEFAULT_N;
  size_t r = DEFAULT_MIN_COMMON_DATA;
  ZZPRF rndZZgen;
  size_t maxLoad = DEFAULT_HASHBUCKETS_MAXLOAD;
  size_t length = DEFAULT_HASHBUCKETS_LENGTH;
  size_t degree;
  HashAlgorithm<NTL::ZZ_p>* hashAlgorithm = nullptr;
  HashBuckets<NTL::ZZ_p>* hashBucketsAlice = nullptr;
  HashBuckets<NTL::ZZ_p>* hashBucketsBob = nullptr;
  unsigned int cores, nThreads = 0;
  byte *data = nullptr;
  size_t dataLen;
  SimpleBenchmark bm;
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "b:hj:k:l:n:p:r:")) != -1) {
    switch (op) {
      case 'b':
        if (atol(optarg) <= 0) {
          std::cerr << argv[0] << ". Bits of plain indexes must be positive." << std::endl;
          exit(1);
        }
        plainSetBits = atol(optarg);
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
        
      case 'n':
        if (atol(optarg) <= 0) {
          std::cerr << argv[0] << ". Number of random generated numbers must be positive." << std::endl;
          exit(1);
        }
        n = atol(optarg);
        break;
        
      case 'p':
        if (atol(optarg) <= 0) {
          std::cerr << argv[0] << ". Number of field size bits must be greater than zero." << std::endl;
          exit(1);
        }
        fieldsizeBits = atol(optarg);
        break;
        
      case 'r':
        if (atol(optarg) < 0) {
          std::cerr << argv[0] << ". Number of common elements can't be negative." << std::endl;
          exit(1);
        }
        r = atol(optarg);
        break;
        
      case '?':
        std::cerr << argv[0] << ". Unrecognised option " << argv[op] << std::endl;
      case 'h':
      default:
        printUsage(argv[0]);
        return 1;
    }
  }
  
  // Additional parameter issues
  if (r > n) {
    std::cerr << argv[0] << ". Common elements can not be more than total elements." << std::endl;
    exit(1);
  }
  if (fieldsizeBits < plainSetBits) {
    std::cerr << argv[0] << ". Field size number of bits must be greater than plain index number of bits." << std::endl;
    exit(1);
  }
  
  // Initialise random seed
  srand(time(NULL));
  
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
  if (nThreads > 1) {
    std::cerr << argv[0] << ". Your NTL library is not thread-safe: using only 1 thread." << std::endl;
    nThreads = 1;
  }
#endif
  if (nThreads > 1) {
    std::cerr << argv[0] << ". Multithreading not (yet) implemented." << std::endl;
  }
  nThreads = 1;
  
  // Initialise modulo operations in NTL
  NTL::SetSeed(NTL::to_ZZ(time(NULL)));
  fieldsize = NTL::GenPrime_ZZ(fieldsizeBits, 100);
  if (!NTL::ProbPrime(fieldsize)) {
    std::cerr << argv[0] << ". " << fieldsize << " does not look like a prime number. Aborting..." << std::endl;
    exit(1);
  } else {
    std::cout << argv[0] << ". Random field size set to prime number " << fieldsize << " (" << NTL::NumBits(fieldsize) << ")." << std::endl;
  }
  NTL::ZZ_p::init(fieldsize);
  
  // Print out initial settings
  std::cout << " --- Settings ------------------------" << std::endl;
  std::cout << "         Number of buckets: " << length << std::endl;
  std::cout << "       Size of each bucket: " << maxLoad << std::endl;
  std::cout << "            Plain set bits: " << plainSetBits << std::endl;
  std::cout << "                Field size: " << fieldsize << " (" << fieldsizeBits << " bits)" << std::endl;
  std::cout << "Number of elements per set: " << n << std::endl;
  std::cout << "   Minimum common elements: " << r << std::endl;
  std::cout << " -------------------------------------" << std::endl;
  
  
  // Create parties
  degree = 2*maxLoad + 1;
  try {
    hashAlgorithm = new MurmurHash3(DEFAULT_MURMURHASH_SEED);
    hashBucketsAlice = new HashBuckets<NTL::ZZ_p>(length, maxLoad, hashAlgorithm);
    hashBucketsBob = new HashBuckets<NTL::ZZ_p>(length, maxLoad, hashAlgorithm);
    alice = new EOPSIClient(*hashBucketsAlice, fieldsize, length, maxLoad, degree, "Alice", (byte *) "\0\0\x02", 3);
    bob = new EOPSIClient(*hashBucketsBob, fieldsize, length, maxLoad, degree, "Bob", (byte *) "\0\0\x01", 3);
    cloud = new EOPSIServer(fieldsize, length, maxLoad, degree, "Cloud");
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error allocating memory." << std::endl;
    exit(1);
  }
  
  // Preconditions: parties are communicating each other through secure channels
  authentication(alice, bob, cloud);
  
  /*
   * 0. Alice and Bob stores blinded values into the cloud.
   */
  bm.start();
  minCommonData = randomData(r, plainSetBits);
  bm.step("min-cap");
  uncommonDataA = randomData(n - r, plainSetBits);
  dataAlice = join(minCommonData, r, uncommonDataA, n - r);
  bm.step("set-A");
  uncommonDataB = randomData(n - r, plainSetBits);
  dataBob = join(minCommonData, r, uncommonDataB, n - r);
  bm.step("set-B");
  
  // Generation stats
  std::cout << EOPSI_MESSAGE_OUTSOURCING_DATA << ". Generating minimum commond data required " << bm.benchmark("min-cap").count()/1000. << " ms" << std::endl;
  std::cout << EOPSI_MESSAGE_OUTSOURCING_DATA << ". Generating random data for A required " << bm.benchmark("set-A").count()/1000. << " ms" << std::endl;
  std::cout << EOPSI_MESSAGE_OUTSOURCING_DATA << ". Generating random data for B required " << bm.benchmark("set-B").count()/1000. << " ms" << std::endl;
  
  msgStoreDataAlice.setType(EOPSI_MESSAGE_OUTSOURCING_DATA);
  msgStoreDataBob.setType(EOPSI_MESSAGE_OUTSOURCING_DATA);
  msgStoreDataAlice.setPartyId(alice->getId());
  msgStoreDataBob.setPartyId(bob->getId());
  bm.step();
  alice->setRawData(dataAlice, n, plainSetBits, nThreads);
  bm.step("add-n-blind-A");
  bob->setRawData(dataBob, n, plainSetBits, nThreads);
  bm.step("add-n-blind-B");
  
  // Blinding stats
  std::cout << EOPSI_MESSAGE_OUTSOURCING_DATA << ". Blinding A's data required " << bm.benchmark("add-n-blind-A").count()/1000. << " ms" << std::endl;
  std::cout << EOPSI_MESSAGE_OUTSOURCING_DATA << ". Blinding B's data required " << bm.benchmark("add-n-blind-B").count()/1000. << " ms" << std::endl;
  
  msgStoreDataAlice.setData(alice->getBlindedData(), alice->getBlindedDataSize());
  msgStoreDataBob.setData(bob->getBlindedData(), bob->getBlindedDataSize());
  bm.step("eopsi-begin");
  try {
    alice->send(*cloud, &msgStoreDataAlice);
    bob->send(*cloud, &msgStoreDataBob);
  } catch (ProtocolException &e) {
    std::cerr << argv[0] << ". " << e.what() << std::endl;
    exit(2);
  }
  bm.step("outsourcing");
  
  // Outsourcing stats
  std::cout << EOPSI_MESSAGE_OUTSOURCING_DATA << ". Outsourcing both A's and B's data to cloud required " << bm.benchmark("outsourcing").count()/1000. << " ms" << std::endl;
  
  /*
   * The right solution of set intersection is here:
   *    setcap with length setcapCard
   * 
   * Use later for check test the correctness of the output
   */
  bm.step();
  setcap = intersect(dataAlice, n, dataBob, n, &setcapCard);
  bm.step("plain-cap");
  
//   std::cout << "Alice data: " << std::endl;
//   NTL::printZZArray(dataAlice, n);
//   std::cout << "Bob data: " << std::endl;
//   NTL::printZZArray(dataBob, n);
//   std::cout << "Expected intersection: " << std::endl;
//   NTL::printZZArray(setcap, setcapCard);
  std::cout << "Size of intersection: " << setcapCard << std::endl;
  
  
  /*
   * 1. B outsources some elaboration to A
   */
  msgBobAlice.setType(EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST);
  msgBobAlice.setPartyId(bob->getId());
  try {
    // Prepare message
    dataLen = NTL::bytes(bob->getSecret()) + 1 + bob->getId().length() + 1;
    data = new byte[dataLen];
    strcpy((char *) data, &bob->getId()[0]);
    NTL::BytesFromZZ(data + bob->getId().length() + 1, bob->getSecret(), NTL::bytes(bob->getSecret()));
    data[dataLen - 1] = '\0'; // Correctly end data
    msgBobAlice.setData(data, dataLen);
    
    // Send the message
    std::cout << bob->getId() << ". Sending client computation request to " << alice->getId() << "." << std::endl;
    bm.step("comp-req");
    
    // Step stats
    std::cout << EOPSI_MESSAGE_CLIENT_COMPUTATION_REQUEST << ". Computation request required " << bm.benchmark("comp-req").count()/1000. << " ms" << std::endl;
    
    bob->send(*alice, &msgBobAlice);
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error allocating memory." << std::endl;
    exit(1);
  } catch (ProtocolException &e) {
    std::cerr << argv[0] << ". " << e.what() << std::endl;
    exit(2);
  }
  bm.step("the-end");
  
  // Compare protocol intersection with known intersection for correctness
  std::cout << argv[0] << ". Checking protocol intersection... ";
  if (checkIntersectionCorrectness(setcap, setcapCard, bob->getIntersection())) {
    std::cout << " correct.";
  } else {
    std::cout << " WRONG!";
  }
  std::cout << std::endl;
  bm.stop("check-result");
  
  // Show benchmarks
  std::cout << " --- Plaintext intersection required " << bm.benchmark("plain-cap").count()/1000. << " ms" << std::endl;
  std::cout << " --- EO-PSI protocol required " << (bm.cumulativeBenchmark("the-end") - bm.cumulativeBenchmark("eopsi-begin")).count()/1000000. << " s" << std::endl;
  std::cout << " --- Checking correctness required " << bm.benchmark("check-result").count()/1000. << " ms" << std::endl;
  
  delete [] dataAlice;
  delete [] dataBob;
  delete alice;
  delete bob;
  delete cloud;
  
  delete hashAlgorithm;
  delete hashBucketsAlice;
  delete hashBucketsBob;
  
  delete [] minCommonData;
  delete [] uncommonDataA;
  delete [] uncommonDataB;
  delete [] setcap;
  delete [] data;
  return 0;
}
//-----------------------------------------------------------------------------
