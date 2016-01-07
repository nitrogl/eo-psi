/*
 * A random generator abstract function
 * 
 * Copyright (C) 2015  Roberto Metere, Glasgow <roberto.metere@strath.ac.uk>
 */

#ifndef RANDOM_GENERATOR_TEMPLATE
#define RANDOM_GENERATOR_TEMPLATE
//-----------------------------------------------------------------------------

/**
 * Random generator is an abstract template class for generic types.
 */
template <class T> class RandomGenerator
{
protected:
  T seed; ///< The seed of generation.
  
public:
  RandomGenerator() {}
  
  /**
   * Initialisation with a specific seed.
   * 
   * @param seed The seed of the generation.
   */
  RandomGenerator(const T &seed) : RandomGenerator() {
    this->setSeed(seed);
  }
  virtual ~RandomGenerator() {}
  
  /**
   * Set the generation seed.
   * 
   * @param seed The seed used for the random generation.
   */
  virtual void setSeed(const T &seed) = 0;
  
  /**
   * Get the next random number.
   */
  virtual T next() = 0;
};
//-----------------------------------------------------------------------------

#endif // RANDOM_GENERATOR_TEMPLATE
