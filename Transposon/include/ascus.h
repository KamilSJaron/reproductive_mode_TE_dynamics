// *********************************************************************
//
// genome.h
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#ifndef ASCUS_H_EDOLGIN_TE
#define ASCUS_H_EDOLGIN_TE

#include "chromosome.h"
#include "random.h"

#include <vector>

class Ascus {

public:
  Ascus();
  Ascus(int, int);
  Ascus(const Ascus &);
  ~Ascus();

  static void SetParameters();

  static unsigned int GetNumberOfChromosomes();
  static unsigned int GetPloidy();
  static double GetFAF();

  unsigned int GetAscusTECount() const;
  unsigned int GetAscusTECountAffectingFitness() const;
  const Chromosome & GetChromosome(int, int) const;
  Chromosome & GetChromosome(int, int);
  double GetAscusFitness() const;
  double GetAscusFitness(int) const;

  void SetChromosome(Chromosome&);
//  void SetAscusParameters(int, int);

  void Transpose();
  void Transpose(double, double);
  void ElementLoss();
  void Recombination();
  //Ascus MakeAscus();
  void ListAscusSites() const;

public:

  // class variables
//  static int N;					// Population size
  static double sa;				// selection coefficient alpha under synergistic epistasis
  static double sb;				// selection coefficient beta under synergistic epistasis
  static double ut;				// transposition rate
  static double vt;				// rate of element loss
  static double faf;			// fraction affecting fitness (FAF)
  //static double rAscus;		// genome wide recombination rate between TE sites
  static int initialTE;
  static int numberOfChromosomes;
  static int ploidy;
  static int chromLengths[];
  static double chromRecRates[];
  static bool parametersSet;

private:

  static Random rand;
  std::vector<Chromosome> chromoVector;

};


#endif