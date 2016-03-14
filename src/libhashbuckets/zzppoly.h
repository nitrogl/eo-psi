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
  NTL::ZZ_pX *polynomials;               ///< The polynomials
  size_t n;                              ///< The number of polynomials in use
  size_t padsize;                        ///< The pad size; very important, it determines the validity of final solutions
  NTL::vec_pair_ZZ_pX_long *factorPairs; ///< The solution of factoring
  NTL::vec_ZZ roots;                     ///< The final intersection
  size_t nThreads;                       ///< The number of threads to use.

public:
  /**
   * Constructor with pad size.
   * The pad size is very important since it determines the validity of the
   * final intersection elements, throwing away not conforming solutions.
   * 
   * @param padsize the pad size to validate intersection elements
   */
  ZZpPolynomials(const size_t padsize);
  
  /**
   * Destructor.
   */
  virtual ~ZZpPolynomials();
  
  /**
   * Use a specific number of threads.
   * TODO: this function currently does nothing, due to multithreading not implemented.
   * 
   * @param n the number of threads to use
   */
  virtual void useThreads(const size_t n);
  
  /**
   * Return the number of polynomials currently in use
   */
  virtual size_t size();
  
  /**
   * Make all polynomials monic
   */
  virtual void makeMonic();
  
  /**
   * Generate n polynomials f by interpolating f(x) = y.
   * The polynomials are stored in an internal field.
   * 
   * @param x the vector of arguments of the polynomial
   * @param y the vector of vectors of polynomial values y, corresponding to unknowns x, y = f(x), for each polynomial
   * @param n the number of polynomials to interpolate
   * @param monicise call monicise after interpolation if <code>true</code>
   */
  virtual void interpolate(NTL::vec_ZZ_p& x, NTL::vec_ZZ_p *y, const size_t n, const bool monicise = true);
  
  /**
   * Factorise polynomials. A side effect is that the factorisation solutions
   * are stored in an internal field.
   * WARNING: call this function after interpolate
   * 
   * @param pfa the polynomial factoring algorithm to use (Berlekamp or CanZass)
   * @see interpolate
   */
  virtual void factorise(const PolynomialFactoringAlgorithm pfa = PFA_CANZASS);
  
  /**
   * Find intersection as valid solutions in factor pairs.
   * WARNING: call this function after factorise.
   * 
   * @return a vector filled with intersection found
   * @see factorise
   */
  virtual NTL::vec_ZZ& findIntersection();
  
  /**
   * Output
   */
  friend std::ostream& operator<<(std::ostream& os, const ZZpPolynomials& polys);
};
//-----------------------------------------------------------------------------

#endif // ZZP_POLYNOMIAL_H
