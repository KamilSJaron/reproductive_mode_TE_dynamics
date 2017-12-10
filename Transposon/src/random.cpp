// *********************************************************************
//
// random.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: April 6, 2005
// Last edited:
//
// *********************************************************************


#include "random.h"
#include "time.h"
#include "math.h"

Random::Random()
{
	seed = time(NULL);
	rng.seed(seed);
	rng_open.seed(seed/2);
}

Random::~Random()
{}

double Random::Uniform()
{
	return rng();
}

double gammalog(double xx)
{
  static double cof[6]={
    76.18009172947146, -86.50532032941677, 24.01409824083091,
    -1.231739572450155, 0.1208650973866179e-2, -0.5395239384953e-5};

  double x = xx;
  double y = xx;
  double tmp = x + 5.5;
  tmp -= (x + 0.5) * log(tmp);
  double ser=1.000000000190015;
  for (int j=0; j<=5; j++)
    ser += cof[j] / ++y;
  return -tmp + log(2.5066282746310005 * ser / x);
}

double Random::Poisson(double lambda)
{
  static double sq, alxm, g, oldm = -1.0;
  double em, t, y;
  const double PI = 3.1415926535897932;

  if (lambda < 12.0) {
    if (lambda != oldm) {
      oldm = lambda;
      g = exp(-lambda);
    }
    em = -1;
    t = 1.0;
    do {
      ++em;
      t *= rng_open();
    } while (t > g);
  } else {
    if (lambda != oldm) {
      oldm = lambda;
      sq = sqrt(2.0 * lambda);
      alxm = log(lambda);
      g = lambda * alxm - gammalog(lambda + 1.0);
    }
    do {
      do {
	y = tan(PI * rng_open());
	em = sq * y + lambda;
      } while (em < 0.0);
      em = floor(em);
      t = 0.9 * (1.0 + y * y) * exp(em * alxm - gammalog(em + 1.0)-g);
    } while (rng_open() > t);
  }
  return em;
}

//double Random::Uniform()
//{
//	return rng.uniform();
//}
//
//double Random::Poisson(double lambda)
//{
//	return rng.poisson(lambda);
//}