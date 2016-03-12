/*
 * Batch operations on polynomials with integer modulo p coefficients
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef ZZP_POLYNOMIAL_H
#define ZZP_POLYNOMIAL_H
//-----------------------------------------------------------------------------

#include <NTL/ZZ_p.h>
#include <NTL/ZZ_pX.h>
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/vec_ZZ_p.h>
#include <NTL/pair_ZZ_pX_long.h>
#include "ntlmiss.h"
//-----------------------------------------------------------------------------

/**
 * Flavour of hash code.
 */
typedef enum PolynomialFactoringAlgorithm {
  PFA_CANZASS = 0, ///< CanZass algorithm
  PFA_BERLEKAMP,   ///< Berlekamp algorithm
} PolynomialFactoringAlgorithm;
//-----------------------------------------------------------------------------

/**
 * This class optimise batch operations on polynomials of of big integers modulo n.
 */
class ZZpPolynomials
{
private:
  NTL::ZZ_pX *polynomials;
  size_t n;
  size_t padsize;
  NTL::vec_pair_ZZ_pX_long *factorPairs;
  NTL::vec_ZZ roots;
  size_t nThreads;

public:
  ZZpPolynomials(const size_t padsize);
  virtual ~ZZpPolynomials();
  
  virtual void useThreads(const size_t n);
  virtual size_t size();
  
  virtual void makeMonic();
  virtual void interpolate(NTL::vec_ZZ_p& x, NTL::vec_ZZ_p *y, const size_t n, const bool monicise = true);
  virtual void factorise(const PolynomialFactoringAlgorithm pfa = PFA_CANZASS);
  virtual NTL::vec_ZZ& findIntersection();
  
  /**
   * Output
   */
  friend std::ostream& operator<<(std::ostream& os, const ZZpPolynomials& polys);
};
//-----------------------------------------------------------------------------

#endif // ZZP_POLYNOMIAL_H
