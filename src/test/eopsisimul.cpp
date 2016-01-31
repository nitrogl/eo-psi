#include <iostream>
#include <cstdlib>
#include <fstream>
#include <getopt.h>
#include <cstdarg>

#include <libhashbuckets.h>
#include "eopsisimul.h"
//-----------------------------------------------------------------------------

static void printUsage(const char *prgnam) {
  std::cout << "Syntax: " << prgnam << " -h -p <modulo> -n <number> -r <number>\n"
            << " -p : set p generating numbers modulo p (big integer)\n"
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
  
  // For each party, authenticate with others
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
static NTL::ZZ_p* join(const NTL::ZZ_p *set1, const size_t card1, const NTL::ZZ_p *set2, const size_t card2) {
  NTL::ZZ_p *res;
  size_t i;
  
  try {
    res = new NTL::ZZ_p[card1 + card2];
  } catch (std::bad_alloc &) {
    std::cerr << "listUnite(). Error allocating memory." << std::endl;
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
static NTL::ZZ_p* intersect(const NTL::ZZ_p *set1, const size_t card1, const NTL::ZZ_p *set2, const size_t card2, size_t *card) {
  NTL::ZZ_p *res, *tmp;
  NTL::ZZ_p *listUnion;
  size_t i, k;
  auto compare = [](const void *a, const void *b) {
    NTL::ZZ n1 = rep(*(NTL::ZZ_p *) a);
    NTL::ZZ n2 = rep(*(NTL::ZZ_p *) b);
    return n1 == n2 ? 0 : (n1 < n2 ? -1 : 1);
  };
  
  // Join and sort them
  listUnion = join(set1, card1, set2, card2);
  qsort(listUnion, card1 + card2, sizeof(NTL::ZZ_p), compare);
  
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
    res = new NTL::ZZ_p[*card];
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
 * This function generates random ZZ_p numbers which are data stored in to 
 * the cloud.
 * 
 * @param n the amount of data to generate
 * @param rndZZpgen the generator
 * @return A pointer to the data which should be manually deleted.
 */
NTL::ZZ_p * randomData(const size_t n, RandomZZpGenerator rndZZpgen) {
  NTL::ZZ_p *data;
  
  try {
    data = new NTL::ZZ_p[n];
  } catch (std::bad_alloc &) {
    std::cerr << "randomData(). Error allocating memory." << std::endl;
    exit(1);
  }
  
  for (size_t i = 0; i < n; i++) {
    data[i] = rndZZpgen.next();
  }
  
  return data;
}
//-----------------------------------------------------------------------------

int main(int argc, char **argv) {
  EOPSIClient *alice, *bob;
  EOPSIServer *cloud;
  EOPSIMessage msgStoreDataAlice, msgStoreDataBob;
  EOPSIMessage msgBobAlice, msgAliceBob, msgAliceCloud, msgCloudBob;
  NTL::ZZ p;
  NTL::ZZ_p seed;
  NTL::ZZ_p *dataAlice, *dataBob, *minCommonData;
  NTL::ZZ_p *setcap;
  size_t setcapCard;
  std::string pstr = DEFAULT_P;
  size_t len;
  size_t n = DEFAULT_N;
  size_t r = DEFAULT_MIN_COMMON_DATA_RATIO;
  RandomStringGenerator rndStrgen;
  RandomZZpGenerator rndZZpgen;
  
  // Parse arguments
  int op = 0; // Return value of getopt_long
  while ((op = getopt(argc, argv, "hn:o:p:r:")) != -1) {
    switch (op) {
      case 'n':
        n = atol(optarg); 
        break;
        
      case 'o':
        break;
        
      case 'p':
        pstr = optarg;
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
  
  // Initialise random ZZ_p generator
  p = NTL::to_ZZ(pstr.c_str());
  NTL::ZZ_p::init(p);
  len = rand() % SEED_MAX_LENGTH + 1;
  rndZZpgen.setModulo(p);
  conv(seed, NTL::ZZFromBytes((const byte*) rndStrgen.next(len).c_str(), len));
  rndZZpgen.setSeed(seed);
  
  // Create parties
  try {
    alice = new EOPSIClient("Alice");
    bob = new EOPSIClient("Bob");
    cloud = new EOPSIServer("Cloud");
  } catch (std::bad_alloc &) {
    std::cerr << argv[0] << ". Error allocating memory." << std::endl;
    exit(1);
  }
  
  // Preconditions: parties are communicating each other through secure channels
  authentication(alice, bob, cloud);
  
  // 0. Alice and Bob stores blinded values into the cloud.
  minCommonData = randomData(r, rndZZpgen);
  dataAlice = join(minCommonData, r, randomData(n - r, rndZZpgen), n - r);
  dataBob = join(minCommonData, r, randomData(n - r, rndZZpgen), n - r);
  msgStoreDataAlice.setType(EOPSI_MESSAGE_OUTSOURCING_DATA);
  msgStoreDataBob.setType(EOPSI_MESSAGE_OUTSOURCING_DATA);
  msgStoreDataAlice.setPartyId(alice->getId());
  msgStoreDataBob.setPartyId(bob->getId());
  try {
    alice->send(*cloud, msgStoreDataAlice);
    bob->send(*cloud, msgStoreDataBob);
  } catch (ProtocolException &e) {
    std::cerr << argv[0] << ". " << e.what() << std::endl;
    exit(1);
  }
  
  /*
   * The right solution of set intersection is here:
   *    setcap with length setcapCard
   * 
   * Use later for check test the correctness of the output
   */
  setcap = intersect(dataAlice, n, dataBob, n, &setcapCard);
  
  delete [] dataAlice;
  delete [] dataBob;
  delete alice;
  delete bob;
  delete cloud;
  return 0;
}
//-----------------------------------------------------------------------------
