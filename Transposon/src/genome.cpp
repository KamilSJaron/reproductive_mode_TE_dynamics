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

#include "../include/genome.h"
#include <math.h>
#include <fstream>
#include <iostream>

//int Genome::N = 0;
double Genome::ut = 0;
double Genome::vt = 0;
double Genome::sa = 0;
double Genome::sb = 0;
double Genome::faf = 0;
int Genome::initialTE = 0;

//double Genome::ut = 0.01;
//double Genome::vt = 0.001;
//double Genome::sa = 0.001;
//double Genome::sb = 0.00018;
//double Genome::faf = 1.0;
//int Genome::initialTE = 50;
//int Genome::N = 0;

int Genome::numberOfChromosomes = 16;
/// This is quite weurd, if I give it 16 numbers it does not work, I do not really get why
int Genome::chromLengths[16] = {200,200,200,200,200,200,200,200};
double Genome::chromRecRates[16] = {0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030};
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
	ut=strtod(tempChar,0);
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
		chromoVector.at(i-1).SetChromNumberAndCopy(i);
		chromoVector.at(i-1).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
	}
}

Genome::Genome(int num) {
	numberOfChromosomes = num;

	chromoVector.resize(numberOfChromosomes);
	if (!parametersSet)
	  SetParameters();

	for (int i=1; i <= numberOfChromosomes; i++) {
		chromoVector.at(i-1).SetChromNumberAndCopy(i);
		chromoVector.at(i-1).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
	}
}

Genome::Genome(const Genome & rhs) {
	chromoVector.resize(numberOfChromosomes);
	if (!parametersSet)
		SetParameters();

	Locus * current;

	for (int i=1; i <= numberOfChromosomes; i++){
		chromoVector.at(i-1).SetChromNumberAndCopy(i);
		chromoVector.at(i-1).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
		current = rhs.GetChromosome(i).GetHeadLocus();
		while (current != 0) {
			chromoVector.at(i-1).Insert(current->GetTransposon());
			current = current->GetNext();
		}
	}
}

unsigned int Genome::GetNumberOfChromosomes() {
	return numberOfChromosomes;
}

double Genome::GetFAF() {
	/// frequency of TEs affecting fitness
	return faf;
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
	  return chromoVector.at(num + 1);
}

Chromosome & Genome::GetChromosome(int num) {
	  return chromoVector.at(num + 1);
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
	unsigned int num=0, pos=0, totalLength=0, currentLength=0;
	bool affectW = false;
	unsigned int teCount = GetGenomeTECount();
	unsigned int transposeCount = (int)rand.Poisson(ut*teCount);

	for (int i=1; i <= numberOfChromosomes; i++)
	  totalLength += GetChromosome(i).GetLength();

	if (transposeCount > teCount)
	  transposeCount = teCount;
	if (transposeCount > (2*totalLength - teCount))
	  transposeCount = (2*totalLength - teCount);

	// for number of transpositions, randomly insert into the genome
	for (int j=0; j < transposeCount; j++) {
		do {
			pos = (int)((rand.Uniform()*totalLength) + 1);
			num = 1;
			for (int k=1; k <= numberOfChromosomes; k++) {
				currentLength = GetChromosome(k).GetLength();
				if (pos > currentLength) {
					num++;
					pos -= currentLength;
				}
				else
				  break;
			}
		} while (!GetChromosome(num).TestEmpty(pos));

		if (faf > rand.Uniform())
			affectW = true;
		else
			affectW = false;
		GetChromosome(num).Insert(Transposon(pos, affectW));
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