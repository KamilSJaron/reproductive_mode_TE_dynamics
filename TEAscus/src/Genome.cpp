// *********************************************************************
//
// genome.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#include <math.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "../include/Genome.h"

#define CHROMOSOMES 16
#define CHROM_LENGTH 500

//int Genome::N = 0;
double Genome::u_initial = 0;
double Genome::vt = 0;
double Genome::sa = 0;
double Genome::sb = 0;
double Genome::faf = 0;
int Genome::initialTE = 0;

const int Genome::numberOfChromosomes = CHROMOSOMES;
const int Genome::chromLength = CHROM_LENGTH;
/// maybe I should turn chromRec to constant as well
double Genome::chromRecRates[16] = {5.6,5.6,5.6,5.6,5.6,5.6,5.6,5.6,5.6,5.6,5.6,5.6,5.6,5.6,5.6,5.6};
//double Genome::rGenome = 0.01;

bool Genome::parametersSet = false;

Random Genome::rand;

void Genome::SetParameters() {
	std::ifstream fin("input.txt");
	if (! fin.is_open())
		{std::cout << "Error opening file"; exit (1); }

	char tempChar[100];
	while(!fin.getline(tempChar, 100).eof())
	{
	// fin.getline(tempChar,100); /// its read somewhere else
//	N=strtol(tempChar,0,10);
	fin.getline(tempChar,100);
	u_initial=strtod(tempChar,0);
	fin.getline(tempChar,100);
	vt=strtod(tempChar,0);
	fin.getline(tempChar,100);
	sa=strtod(tempChar,0);
	fin.getline(tempChar,100);
	sb=strtod(tempChar,0);
	fin.getline(tempChar,100);
	faf=strtod(tempChar,0);
	fin.getline(tempChar,100);
	initialTE=strtol(tempChar,0,10);
	}
	fin.close();
	parametersSet = true;
}

Genome::Genome() {
	chromoVector.resize(numberOfChromosomes);
	if (!parametersSet)
		SetParameters();

	for (int i=1; i <= numberOfChromosomes; i++) {
		chromoVector.at(i-1).SetChromNumber(i);
		chromoVector.at(i-1).SetChromLengthAndRecRate(chromLength, chromRecRates[i-1]);
	}
}

Genome::Genome(const Genome & rhs) {
	chromoVector.resize(numberOfChromosomes);

	if (!parametersSet)
		SetParameters();

	Locus * current;

	for (int i=1; i <= numberOfChromosomes; i++){
		chromoVector.at(i-1).SetChromNumber(i);
		chromoVector.at(i-1).SetChromLengthAndRecRate(chromLength, chromRecRates[i-1]);
		current = rhs.GetChromosome(i).GetHeadLocus();
		while (current != 0) {
			chromoVector.at(i-1).Insert(current->GetTransposon());
			current = current->GetNext();
		}
	}
}

double Genome::GetFAF() {
	/// frequency of TEs affecting fitness
	return faf;
}

int Genome::GenerateNumberOfChiasmas(int chromosome){
	return(rand.Poisson(Genome::chromRecRates[chromosome-1]));
}

/// to be checked if generates what I want
int Genome::GenerateGapPositionOnChromosome(){
	/// supposed to generate int 0 ... 400
	return(round((rand.Uniform() * (chromLength + 1)) - 0.5));
}

void Genome::GenerateChromosomeAndPosition(int * ch, int * p){
	int roll = 0;
	/// 0 ... (chromLength * numberOfChromosomes) - 1
	roll = round((rand.Uniform() * chromLength * numberOfChromosomes) - 0.5);
	/// in range from 1 to chromLength
	*p = (roll % chromLength) + 1;
	/// in range from 1 to numberOfChromosomes
	*ch = (roll / chromLength) + 1;
}

bool Genome::GenerateTossACoin(){
	if (rand.Uniform() < 0.5)
		return(true);
	else
		return(false);
}

unsigned int Genome::GetGenomeTECount() const {
	unsigned int genomeTEcount = 0;
	for (int i=1; i <= numberOfChromosomes; i++) {
		genomeTEcount += GetChromosome(i).GetChromTECount();
	}
	return genomeTEcount;
}

unsigned int Genome::GetGenomeTECountAffectingFitness() const {
	unsigned int genomeTEcount = 0;
	for (int i=1; i <= numberOfChromosomes; i++) {
		genomeTEcount += GetChromosome(i).GetChromTECountAffectingFitness();
	}
	return genomeTEcount;
}

// input is chromosome number and copy number
const Chromosome & Genome::GetChromosome(int num) const {
		return chromoVector.at(num - 1);
}

Chromosome & Genome::GetChromosome(int num) {
		return chromoVector.at(num - 1);
}

double Genome::GetGenomeFitness() const
{
	unsigned int genomeTEcount = GetGenomeTECountAffectingFitness();

	//return (1 - 0.001*pow(genomeTEcount, 1.5));

	// assume synergistic epistatic selection
	return exp ( -(sa * genomeTEcount) - (0.5 * sb * pow(genomeTEcount,2) ) );
}

// overloaded function when mean TE count is constant
double Genome::GetGenomeFitness(int meanCount) const {
	//return (1 - 0.001*pow(meanCount, 1.5));

	// assume synergistic epistatic selection
	return exp ( -(sa * meanCount) - (0.5 * sb * pow(meanCount,2) ) );
}

void Genome::SetChromosome(Chromosome & c) {
	int num = c.GetChromNumber();

	chromoVector.at(num - 1) = c;
}

void Genome::Transpose() {
	// std::cerr << "Transposing" << std::endl;
	int rolled_chromosome = 0, rolled_position_on_ch = 0;
	unsigned int transposeCount = 0;
	unsigned int totalLength = chromLength * numberOfChromosomes;
	Transposon te;

	int TEs = GetGenomeTECount();
	// std::cerr << "Proportion of gneome covered by TEs : " << TEs / (double)totalLength << std::endl;

	Locus * current;
	double transposition_rate = 0;
	bool affectW = true, roll_again = true;
	// std::cerr << "setup" << std::endl;

	for (int ch=1; ch <= numberOfChromosomes; ch++){
		// std::cerr << "ch " << ch << std::endl;
		current = GetChromosome(ch).GetHeadLocus();
		while (current != 0) {
			// roll number of insertions
			// std::cerr << "rolling ";
			transposition_rate = current->GetTranspositionRate();
			transposeCount = (int)rand.Poisson(transposition_rate);
			TEs += transposeCount;
			if ((TEs / (double)totalLength) > 0.8){
				std::cerr << "ERROR : an individual with more 80% of genome covered by TEs in simualtion." << std::endl;
				std::cerr << "Perhaps it would be good to modify parameters to meaningful values." << std::endl;
				exit (EXIT_FAILURE);
			}
			// std::cerr << transposeCount << " using u " << transposition_rate << std::endl;
			for (int t = 0; t < transposeCount; t++) {
				// std::cerr << "transposing locus" << std::endl;
				// roll where
				roll_again = true;
				do {
					Genome::GenerateChromosomeAndPosition(& rolled_chromosome, & rolled_position_on_ch);
					// std::cerr << "stuff rolled, testing" << rolled_chromosome << " at position " << rolled_position_on_ch << std::endl;
					roll_again = !GetChromosome(rolled_chromosome).TestEmpty(rolled_position_on_ch);
					// std::cerr << "CH: " << rolled_chromosome << " TEs: " << GetChromosome(rolled_chromosome).GetChromTECount() << std::endl;
				} while (roll_again);

				// std::cerr << "position rolled" << std::endl;
				// roll for TE adjustment
				// transposition_rate += ????;

				// is new TE going to affect fitness
				if (faf > rand.Uniform())
					affectW = true;
				else
					affectW = false;

				// std::cerr << "taget ch" << rolled_chromosome << " pos " << rolled_position_on_ch << " transposition_rate " << transposition_rate << " affecting fitness? " << affectW << std::endl;
				chromoVector.at(rolled_chromosome - 1).Insert(Transposon(rolled_position_on_ch, transposition_rate, affectW));
				// std::cerr << "taget ch" << rolled_chromosome << " pos " << rolled_position_on_ch << " transposition_rate " << transposition_rate << " affecting fitness? " << affectW << std::endl;
			}
			// std::cerr << "GetNext ";
			current = current->GetNext();
			// std::cerr << "worked" << std::endl;
		}
	}
}

void Genome::ElementLoss()
{
	if (vt == 0){
		return;
	}

	unsigned int lossCount=0, chromTEcount=0, nthTE=0;

	for (int i=1; i <= numberOfChromosomes; i++) {
		chromTEcount = chromoVector.at(i-1).GetChromTECount();
		lossCount = (int)rand.Poisson(vt*chromTEcount);

		/// if by any chance poisson distribution generates number greater than number of TEs
		if (lossCount > chromTEcount) {
			/// just lose all of them
			lossCount = chromTEcount;
		}

		for (int k=0; k < lossCount; k++) {
			nthTE = (int)(rand.Uniform()*chromTEcount + 1);
			chromoVector.at(i-1).Delete(nthTE);
			chromTEcount--;
		}
		}
}

void Genome::ListGenomeSites() const
{
	for (int i=0; i < numberOfChromosomes; i++){
		chromoVector.at(i).ListChromSites();
	}
	std::cout << std::endl;
}

int Genome::GetEmpiricalNumberOfChromosomes() {
	return(chromoVector.size());
}
