// *********************************************************************
//
// random.h
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: April 6, 2005
// Last edited:
//
// *********************************************************************

#ifndef RANDOM_H_EDOLGIN_TE
#define RANDOM_H_EDOLGIN_TE

//#include "rng.h"
#include "mtrand.h"


class Random {

public:
  Random();
  ~Random();

  double Uniform();
  double Poisson(double);

private:
  //RNG rng;
  unsigned int seed;
  MTRand rng;
  MTRand_open rng_open;

  //static const double PI = 3.1415926535897932;
};


#endif