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

#include "../include/Chromosome.h"
// #include "../include/Random.h"

#include <vector>
#include <random>

class Genome {

public:
	Genome();
	Genome(const Genome &);

	static void SetParameters();
	static double GetFAF();
	static int GenerateNumberOfChiasmas(int chromosome);
	static int GenerateGapPositionOnChromosome();
	static void GenerateChromosomeAndPosition(int * ch, int * p);
	static bool GenerateTossACoin();

	unsigned int GetGenomeTECount() const;
	unsigned int GetGenomeTECountAffectingFitness() const;
	const Chromosome & GetChromosome(int) const;
	Chromosome & GetChromosome(int);
	double GetGenomeFitness() const;
	double GetGenomeFitness(int) const;

	void SetChromosome(Chromosome&);

	void Transpose();
	void Transpose(double, double);
	void ElementLoss();
	void Recombination();
	//Ascus MakeAscus();
	void ListGenomeSites() const;

	/// just for testing
	int GetEmpiricalNumberOfChromosomes();

public:

	// class variables
//	static int N;					// Population size
	static double sa;				// selection coefficient alpha under synergistic epistasis
	static double sb;				// selection coefficient beta under synergistic epistasis
	static double u_initial;				// transposition rate
	static double vt;				// rate of element loss
	static double faf;			// fraction affecting fitness (FAF)
	//static double rGenome;		// genome wide recombination rate between TE sites
	static int initialTE;
	const static int numberOfChromosomes;
	const static int chromLength;
	static double chromRecRates[];
	static bool parametersSet;

private:

	// static Random rand;
	static std::random_device rd;
	static std::mt19937 mt;
	static std::normal_distribution<double> rnorm;
	static std::uniform_int_distribution<int> rgap;
	static std::uniform_int_distribution<int> rpos;
	static std::uniform_int_distribution<bool> toss;
	static std::uniform_int_distribution<int> rch;
	
	std::vector<Chromosome> chromoVector;

};


#endif