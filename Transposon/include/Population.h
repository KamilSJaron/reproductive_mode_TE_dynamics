// *********************************************************************
//
// population.h
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 17, 2005
// Last edited:
//
// *********************************************************************

#ifndef POPULATION_H_EDOLGIN_TE
#define POPULATION_H_EDOLGIN_TE

#include "../include/Genome.h"
#include <vector>


class Population {

public:
	Population(int);
	~Population();

	Genome & GetIndividual(int);
	const Genome & GetIndividual(int) const;
	int GetPopSize() const;
	unsigned int GetPopulationTECount() const;
	unsigned int GetPopulationTECountAffectingFitness() const;
	double GetPopulationMeanFitness() const;

	void Initialize();
	int SelectVitalIndividual() const;
	Genome MakeIndividual();
	void DeleteIndividual(int);
	Population * SexualReproduction();
	Population * AsexualReproduction();
	void TranspositionAndLoss();

	void ListPopulationSites() const;
	void PrintParameters(const char *);
	void SummaryStatistics(const char *, int);
	void RecordPopulation(const char *, int);

	void SummaryStatistics(int);
	void SummaryStatistics(int, int);

private:
	std::vector<Genome> genoVector;
	int popSize;
	static Random rand;

	int getLocusPosition(Locus *) const;
	/// sexual offpring - fusion of parents, recombination, creation of two complementary offsprings
	void generateTwoOspring(int ind, Population * newPopulation, Genome & parent1, Genome & parent2);
};


#endif


