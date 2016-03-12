/*
 * Batch operations on polynomials with integer modulo p coefficients
 * 
 * Copyright (C) 2016  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#include <iostream>
#include <sstream>
#include <thread>
#include <thread>
#include "zzppoly.h"
//-----------------------------------------------------------------------------

ZZpPolynomials::ZZpPolynomials(const size_t padsize) {
  this->n = 0;
  this->padsize = padsize;
  this->polynomials = nullptr;
  this->factorPairs = nullptr;
  this->nThreads = 1;
}
//-----------------------------------------------------------------------------

ZZpPolynomials::~ZZpPolynomials() {
  if (this->polynomials != nullptr) {
    delete [] this->polynomials;
  }
}
//-----------------------------------------------------------------------------

void ZZpPolynomials::useThreads(const size_t n) {
  size_t cores;
  
  std::cerr << "useThreads(). not yet implemented." << std::endl;
  if (n == 0) {
    cores = std::thread::hardware_concurrency() > 0 ? std::thread::hardware_concurrency() : 1;
    nThreads = cores;
  } else {
    this->nThreads = n;
  }
}
//-----------------------------------------------------------------------------

size_t ZZpPolynomials::size() {
  return this->n;
}
//-----------------------------------------------------------------------------

void ZZpPolynomials::makeMonic() {
  for (size_t j = 0; j < this->n; j++) {
    NTL::MakeMonic(this->polynomials[j]);
//     std::cout << "Monicised: {" << polynomials[j] << "}" << std::endl;
  }
}
//-----------------------------------------------------------------------------

void ZZpPolynomials::interpolate(NTL::vec_ZZ_p& x, NTL::vec_ZZ_p *y, const size_t n, const bool monicise) {
  if (this->n != n) {
    try {
      if (this->polynomials != nullptr) {
        delete [] this->polynomials;
      }
      
      this->polynomials = new NTL::ZZ_pX[n];
    } catch (std::bad_alloc &) {
      std::cerr << "interpolate(). Error allocating memory." << std::endl;
      exit(2);
    }
  }
  
  this->n = n;
  for (size_t j = 0; j < n; j++) {
//     std::cout << "\t------------" << std::endl;
    polynomials[j] = NTL::interpolate(x, y[j]);
//     std::cout << "Polynomial " << j << ": {" << polynomials[j] << "}" << std::endl;
  }
  
  if (monicise) {
    this->makeMonic();
  }
}
//-----------------------------------------------------------------------------

void ZZpPolynomials::factorise(const PolynomialFactoringAlgorithm pfa) {
  void (*fact)(NTL::vec_pair_ZZ_pX_long&, const NTL::ZZ_pX&, long);
  
  if (this->polynomials == nullptr) {
    std::cerr << "factorise(). No polynomials to factorise, you should interpolate first." << std::endl;
    return;
  }
  
  // Choose algorithm
  switch (pfa) {
    case PFA_BERLEKAMP:
      fact = &NTL::berlekamp;
      break;
    
    default:
      std::cerr << "factorise(). WARNING: Unknown polynomial algorithm provided, default to CanZass." << std::endl;
    case PFA_CANZASS:
      fact = &NTL::CanZass;
      break;
  }
  
  // Allocate space
  if (this->factorPairs == nullptr) {
    if (this->factorPairs == nullptr) {
      delete [] this->factorPairs;
    }
    
    try {
      factorPairs = new NTL::vec_pair_ZZ_pX_long[this->n];;
    } catch (std::bad_alloc &) {
      std::cerr << "factorise(). Error allocating memory." << std::endl;
      exit(2);
    }
  }
  
  // Factoring
  for (size_t j = 0; j < this->n; j++) {
//     factorPairs[j] = berlekamp(polynomials[j]);
    fact(this->factorPairs[j], this->polynomials[j], 0);
//     std::cout << "FactorPairs: {" << factorPairs[j] << "}" << std::endl;
  }
}
//-----------------------------------------------------------------------------

NTL::vec_ZZ& ZZpPolynomials::findIntersection() {
  NTL::ZZ z;
  
  if (this->factorPairs == nullptr) {
    std::cerr << "findIntersection(). You have to factorise first." << std::endl;
    this->roots.SetLength(0);
    return this->roots;
  }
  
  // Filter only acceptable solutions
  this->roots.SetLength(0);
  for (size_t j = 0; j < this->n; j++) {
    for(size_t k = 0; k < (size_t) this->factorPairs[j].length(); k++) {
      if (NTL::deg(this->factorPairs[j][k].a) == 1) {
        z = rep(-(NTL::ConstTerm(this->factorPairs[j][k].a)));
        
//         std::cout << "Result " << k << ": " << z << " - Leading zeros: " << NTL::countLeadingZeros(z) << std::endl;
        if (NTL::countLeadingZeros(z) >= this->padsize) {
          NTL::append(this->roots, z);
        }
      }
    }
  }
  
  return this->roots;
}
//-----------------------------------------------------------------------------

std::ostream& operator<<(std::ostream& os, const ZZpPolynomials& polys) {
    os << "ZZpPolynomials ";
    if (polys.n) {
      os << "contains " << polys.n << " polynomials.";
    } else {
      os << "is empty!";
    }
    
    return os;
}
//-----------------------------------------------------------------------------
