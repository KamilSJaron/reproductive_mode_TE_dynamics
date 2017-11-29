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

#ifndef GENOME_H_EDOLGIN_TE
#define GENOME_H_EDOLGIN_TE

#include "chromosome.h"
#include "random.h"

#include <vector>

class Genome {

public:
  Genome();
  Genome(int, int);
  Genome(const Genome &);
  ~Genome();
  
  static void SetParameters();
  
  static unsigned int GetNumberOfChromosomes();
  static unsigned int GetPloidy();
  static double GetFAF();
  
  unsigned int GetGenomeTECount() const;
  unsigned int GetGenomeTECountAffectingFitness() const;
  const Chromosome & GetChromosome(int, int) const;
  Chromosome & GetChromosome(int, int);
  double GetGenomeFitness() const;
  double GetGenomeFitness(int) const;
  
  void SetChromosome(Chromosome&);
//  void SetGenomeParameters(int, int);
  
  void Transpose();
  void Transpose(double, double);
  void ElementLoss();
  void Recombination();
  //Genome MakeGamete();
  void ListGenomeSites() const;
  
public:

  // class variables
//  static int N;					// Population size
  static double sa;				// selection coefficient alpha under synergistic epistasis
  static double sb;				// selection coefficient beta under synergistic epistasis
  static double ut;				// transposition rate
  static double vt;				// rate of element loss
  static double faf;			// fraction affecting fitness (FAF)
  //static double rGenome;		// genome wide recombination rate between TE sites
  static int initialTE;
  static int numberOfChromosomes;
  static int ploidy;
  static int chromLengths[];
  static double chromRecRates[];
  static bool clonal;
  static bool parametersSet;

private:

  static Random rand;
  std::vector<Chromosome> chromoVector;
  
};


#endif