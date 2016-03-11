#include <iostream>
#include <cstdlib>
#include <fstream>
#include <getopt.h>
#include <cstdarg>

#include <libhashbuckets.h>
#include "eopsisimul.h"
//-----------------------------------------------------------------------------

/**
 * Prints the usage of this executable
 * 
 * @param prgnam the name of the program/binary called
 */
static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -h -b <bits-in-plain-sets> -p <field-size> -n <number> -r <number>\n"
            << " -j : force number of threads for evaluation (be sure your NTL is thread-safe)\n"
            << " -k : number of keys of the hash table\n"
            << " -l : size of buckets of the hash table\n"
            << " -p : set the padding up to numbers modulo p\n"
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
 * This function computes the intersection of two sets, with no duplicates.
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
  
  try {
    data = new NTL::ZZ[n];
  } catch (std::bad_alloc &) {
    std::cerr << "randomData(). Error allocating memory." << std::endl;
    exit(1);
  }
  
  for (size_t i = 0; i < n; i++) {
    data[i] = NTL::RandomBits_ZZ(bitsize);
    for (size_t j = 0; j < i - 1 && i != 0; j++) {
      if (data[i] == data[j]) {
        i--;
        break;
      }
    }
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
  NTL::ZZ *dataAlice, *dataBob, *minCommonData;
  NTL::ZZ *setcap;
  size_t setcapCard;
  std::string plainSetBitsStr = DEFAULT_PLAIN_SET_BITS;
  std::string fieldsizeStr = DEFAULT_P;
  size_t plainSetBits;
  size_t n = DEFAULT_N;
  size_t r = DEFAULT_MIN_COMMON_DATA;
  ZZPRF rndZZgen;
  size_t maxLoad = DEFAULT_HASHBUCKETS_MAXLOAD;
  size_t length = DEFAULT_HASHBUCKETS_LENGTH;
  size_t degree;
  HashAlgorithm<NTL::ZZ_p>* hashAlgorithm = nullptr;
  HashBuckets<NTL::ZZ_p>* hashBucketsAlice = nullptr;
  HashBuckets<NTL::ZZ_p>* hashBucketsBob = nullptr;
  unsigned int nThreads = 0;
  byte *data = nullptr;
  size_t dataLen;
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "b:hj:n:o:p:r:")) != -1) {
    switch (op) {
      case 'b':
        plainSetBitsStr = optarg;
        break;
        
      case 'j':
        nThreads = atoi(optarg);
        break;
        
      case 'n':
        n = atol(optarg);
        break;
        
      case 'o':
        break;
        
      case 'p':
        fieldsizeStr = optarg;
        break;
        
      case 'r':
        r = atol(optarg);
        break;
        
      case '?':
        std::cerr << "Unrecognised option " << argv[op] << std::endl;
      case 'h':
      default:
        printUsage(argv[0]);
        return 1;
    }
  }
  
  // Initialise random seed
  srand(time(NULL));
  
  // Number of threads
  if (nThreads == 0) {
    cores = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1;
    nThreads = cores;
    nThreads = 1; // [[REM]] Default to 1, due to old NTL being not thread safe
  }
  
  // Number of bits of generated numbers
  plainSetBits = atol(plainSetBitsStr.c_str());
  
  // Initialise modulo operations in NTL
  fieldsize = NTL::to_ZZ(fieldsizeStr.c_str());
  NTL::ZZ_p::init(fieldsize);
  
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
  minCommonData = randomData(r, plainSetBits);
  dataAlice = join(minCommonData, r, randomData(n - r, plainSetBits), n - r);
  dataBob = join(minCommonData, r, randomData(n - r, plainSetBits), n - r);
  
  msgStoreDataAlice.setType(EOPSI_MESSAGE_OUTSOURCING_DATA);
  msgStoreDataBob.setType(EOPSI_MESSAGE_OUTSOURCING_DATA);
  msgStoreDataAlice.setPartyId(alice->getId());
  msgStoreDataBob.setPartyId(bob->getId());
  alice->setRawData(dataAlice, n, nThreads);
  bob->setRawData(dataBob, n, nThreads);
  
  msgStoreDataAlice.setData(alice->getBlindedData(), alice->getBlindedDataSize());
  msgStoreDataBob.setData(bob->getBlindedData(), bob->getBlindedDataSize());
  try {
    alice->send(*cloud, &msgStoreDataAlice);
    bob->send(*cloud, &msgStoreDataBob);
  } catch (ProtocolException &e) {
    std::cerr << argv[0] << ". " << e.what() << std::endl;
    exit(2);
  }
  
  /*
   * The right solution of set intersection is here:
   *    setcap with length setcapCard
   * 
   * Use later for check test the correctness of the output
   */
  setcap = intersect(dataAlice, n, dataBob, n, &setcapCard);
  
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
    bob->send(*alice, &msgBobAlice);
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error allocating memory." << std::endl;
    exit(1);
  } catch (ProtocolException &e) {
    std::cerr << argv[0] << ". " << e.what() << std::endl;
    exit(2);
  }
  
//   delete [] dataAlice;
//   delete [] dataBob;
//   delete alice;
//   delete bob;
//   delete cloud;
  return 0;
}
//-----------------------------------------------------------------------------
