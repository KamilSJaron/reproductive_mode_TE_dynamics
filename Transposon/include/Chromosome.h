// *********************************************************************
//
// chromosome.h
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#ifndef CHROMOSOME_H_EDOLGIN_TE
#define CHROMOSOME_H_EDOLGIN_TE

#include "locus.h"


class Chromosome
{
  public:
	Chromosome();
	Chromosome(int, int, int, double);
	~Chromosome();

	unsigned int GetChromTECount() const;
	unsigned int GetChromTECountAffectingFitness() const;
	unsigned int GetChromNumber() const;
	unsigned int GetChromCopy() const;
	unsigned int GetLength() const;
	double GetRecRate() const;
	Locus * GetHeadLocus() const;

	void SetHeadLocus(Locus *);
	void SetChromNumberAndCopy(int);
	void SetChromLengthAndRecRate(int, double);

	bool TestEmpty(int) const;

	void Insert(Transposon);
	void Delete(int);

	void ListChromSites() const;


  private:
	Locus * headLocus;

	unsigned int chromNumber;	// chromosome number
	unsigned int chromCopy;		// denotes ploidy
	unsigned int length;

	double rChrom;				// recombination rate between TE sites

};



#endif