/*
 * A random generator abstract function
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H
//-----------------------------------------------------------------------------

template <class T> class RandomGenerator
{
protected:
  T seed;
  
public:
  RandomGenerator() {}
  RandomGenerator(const T &seed) : RandomGenerator() {
    this->setSeed(seed);
  }
  virtual ~RandomGenerator() {}
  
  virtual void setSeed(const T &seed) = 0;
  virtual T next() const = 0;
};
//-----------------------------------------------------------------------------

#endif // RANDOM_GENERATOR_H
