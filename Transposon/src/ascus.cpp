// *********************************************************************
//
// Ascus.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 11, 2005
// Last edited:
//
// *********************************************************************

#include "../include/ascus.h"
#include <math.h>
#include <fstream>
#include <iostream>

//int Ascus::N = 0;
double Ascus::ut = 0;
double Ascus::vt = 0;
double Ascus::sa = 0;
double Ascus::sb = 0;
double Ascus::faf = 0;
int Ascus::initialTE = 0;

//double Ascus::ut = 0.01;
//double Ascus::vt = 0.001;
//double Ascus::sa = 0.001;
//double Ascus::sb = 0.00018;
//double Ascus::faf = 1.0;
//int Ascus::initialTE = 50;
//int Ascus::N = 0;

int Ascus::numberOfChromosomes = 16;
int Ascus::ploidy = 2;
int Ascus::chromLengths[16] = {200,200,200,200,200,200,200,200,200,200,200,200,200,200,200,200};
double Ascus::chromRecRates[16] = {0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030,0.030};
//double Ascus::rAscus = 0.01;

bool Ascus::parametersSet = false;

Random Ascus::rand;

void Ascus::SetParameters()
{
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

Ascus::Ascus()
{
	chromoVector.resize((numberOfChromosomes*ploidy));
	if (!parametersSet)
	  SetParameters();

	// for (int i=1; i <= numberOfChromosomes; i++) {
	// 	if (ploidy == 1) {
	// 		chromoVector.at(i-1).SetChromNumberAndCopy(i,1);
	// 		chromoVector.at(i-1).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
	// 	}
	// 	if (ploidy == 2) {
	// 		chromoVector.at(2*i-2).SetChromNumberAndCopy(i,1);
	// 		chromoVector.at(2*i-2).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
	// 		chromoVector.at(2*i-1).SetChromNumberAndCopy(i,2);
	// 		chromoVector.at(2*i-1).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
	// 	}
    // }
}

Ascus::Ascus(int num, int pl)
{
	numberOfChromosomes = num;
	ploidy = pl;

	chromoVector.resize((numberOfChromosomes*ploidy));
	if (!parametersSet)
	  SetParameters();

	// for (int i=1; i <= numberOfChromosomes; i++)
	// {
	//   if (ploidy == 1)
	//   {
	// 	chromoVector.at(i-1).SetChromNumberAndCopy(i,1);
	// 	chromoVector.at(i-1).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
	//   }
	//   if (ploidy == 2)
	//   {
	//     chromoVector.at(2*i-2).SetChromNumberAndCopy(i,1);
	// 	chromoVector.at(2*i-2).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
	// 	chromoVector.at(2*i-1).SetChromNumberAndCopy(i,2);
	// 	chromoVector.at(2*i-1).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
	//   }
    // }
}

Ascus::Ascus(const Ascus & rhs)
{
	chromoVector.resize((numberOfChromosomes*ploidy));
	if (!parametersSet)
	  SetParameters();

	Locus * current;

	for (int i=1; i <= numberOfChromosomes; i++)
	{
	  if (ploidy == 1)
	  {
		// chromoVector.at(i-1).SetChromNumberAndCopy(i,1);
		chromoVector.at(i-1).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
		current = rhs.GetChromosome(i,1).GetHeadLocus();
		while (current != 0)
		{
			chromoVector.at(i-1).Insert(current->GetTransposon());
			current = current->GetNext();
		}
	  }
	  if (ploidy == 2)
	  {
	    // chromoVector.at(2*i-2).SetChromNumberAndCopy(i,1);
		chromoVector.at(2*i-2).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
		current = rhs.GetChromosome(i,1).GetHeadLocus();
		while (current != 0)
		{
			chromoVector.at(2*i-2).Insert(current->GetTransposon());
			current = current->GetNext();
		}
		// chromoVector.at(2*i-1).SetChromNumberAndCopy(i,2);
		chromoVector.at(2*i-1).SetChromLengthAndRecRate(chromLengths[i-1], chromRecRates[i-1]);
		current = rhs.GetChromosome(i,2).GetHeadLocus();
		while (current != 0)
		{
			chromoVector.at(2*i-1).Insert(current->GetTransposon());
			current = current->GetNext();
		}
	  }
    }
}

Ascus::~Ascus()
{}

unsigned int Ascus::GetNumberOfChromosomes()
{
	return numberOfChromosomes;
}

unsigned int Ascus::GetPloidy()
{
	return ploidy;
}

double Ascus::GetFAF()
{
	return faf;
}

unsigned int Ascus::GetAscusTECount() const
{
	unsigned int AscusTEcount = 0;
	for (int i=1; i <= numberOfChromosomes; i++)
	{
	  AscusTEcount += GetChromosome(i, 1).GetChromTECount();
	  if (ploidy==2)
		AscusTEcount += GetChromosome(i, 2).GetChromTECount();
	}
	return AscusTEcount;
}

unsigned int Ascus::GetAscusTECountAffectingFitness() const
{
	unsigned int AscusTEcount = 0;
	for (int i=1; i <= numberOfChromosomes; i++)
	{
	  AscusTEcount += GetChromosome(i, 1).GetChromTECountAffectingFitness();
	  if (ploidy==2)
	    AscusTEcount += GetChromosome(i, 2).GetChromTECountAffectingFitness();
	}
	return AscusTEcount;
}

// input is chromosome number and copy number
const Chromosome & Ascus::GetChromosome(int num, int copy) const
{
	if (ploidy == 1)
	  return chromoVector.at(num+copy-2);
	else // if (ploidy == 2)
	  return chromoVector.at((2*num)+copy-3);
}

Chromosome & Ascus::GetChromosome(int num, int copy)
{
	if (ploidy == 1)
	  return chromoVector.at(num+copy-2);
	else // if (ploidy == 2)
	  return chromoVector.at((2*num)+copy-3);
}

double Ascus::GetAscusFitness() const
{
	unsigned int AscusTEcount = GetAscusTECountAffectingFitness();

	//return (1 - 0.001*pow(AscusTEcount, 1.5));

	// assume synergistic epistatic selection
	return exp ( -(sa * AscusTEcount) - (0.5 * sb * pow(AscusTEcount,2) ) );
}

// overloaded function when mean TE count is constant
double Ascus::GetAscusFitness(int meanCount) const
{
	//return (1 - 0.001*pow(meanCount, 1.5));

	// assume synergistic epistatic selection
	return exp ( -(sa * meanCount) - (0.5 * sb * pow(meanCount,2) ) );
}

void Ascus::SetChromosome(Chromosome & c) {
	int num = c.GetChromNumber();
	int copy = c.GetChromCopy();

	if (ploidy == 1)
	  chromoVector.at(num+copy-2) = c;
	if (ploidy == 2)
	  chromoVector.at((2*num)+copy-3) = c;
}

void Ascus::Transpose() {
	unsigned int num=0, copy=0, pos=0, totalLength=0, currentLength=0;
	bool affectW = false;
	unsigned int teCount = GetAscusTECount();
	unsigned int transposeCount = (int)rand.Poisson(ut*teCount);

	for (int i=1; i <= numberOfChromosomes; i++)
	  totalLength += GetChromosome(i, 1).GetLength();

	if (transposeCount > teCount)
	  transposeCount = teCount;
	if (transposeCount > (2*totalLength - teCount))
	  transposeCount = (2*totalLength - teCount);

	// for number of transpositions, randomly insert into the Ascus
	for (int j=0; j < transposeCount; j++)
	{
	  do {
	    pos = (int)((rand.Uniform()*totalLength) + 1);
		num = 1;
		for (int k=1; k <= numberOfChromosomes; k++)
		{
			currentLength = GetChromosome(k, 1).GetLength();
			if (pos > currentLength)
			{
				num++;
				pos -= currentLength;
			}
			else
			  break;
		}
		copy = (int)((rand.Uniform())*(ploidy) + 1);
	  } while (!GetChromosome(num, copy).TestEmpty(pos));

		if (faf > rand.Uniform())
		  affectW = true;
		else
		  affectW = false;

		GetChromosome(num, copy).Insert(Transposon(pos, affectW));
	}
}

// Overloaded Transpose method used for when at equilibrium copy number
void Ascus::Transpose(double rate, double meanCopyNumber)
{
	int num=0, copy=0, pos=0, currentLength=0, totalLength=0;
	bool affectW = false;
	unsigned int transposeCount = (int)rand.Poisson(rate*meanCopyNumber);

	for (int i=1; i <= numberOfChromosomes; i++)
	  totalLength += GetChromosome(i, 1).GetLength();

	if (transposeCount > meanCopyNumber)
	  transposeCount = (int)meanCopyNumber;
	if (totalLength < (transposeCount + meanCopyNumber))
	  transposeCount = (totalLength - (int)meanCopyNumber);

	// for number of transpositions, randomly insert into the Ascus
	for (int j=0; j < transposeCount; j++)
	{
	  do {
	    pos = (int)((rand.Uniform()*totalLength) + 1);
		num = 1;
		for (int k=1; k <= numberOfChromosomes; k++)
		{
			currentLength = GetChromosome(k, 1).GetLength();
			if (pos > currentLength)
			{
				num++;
				pos -= currentLength;
			}
			else
			  break;
		}
		copy = (int)((rand.Uniform())*(ploidy) + 1);
	  } while (!GetChromosome(num, copy).TestEmpty(pos));

		if (faf > rand.Uniform())
		  affectW = true;
		else
		  affectW = false;

		GetChromosome(num, copy).Insert(Transposon(pos, affectW));
	}
}

void Ascus::ElementLoss()
{
	if (vt == 0)
	  return;

	unsigned int lossCount=0, chromTEcount=0, nthTE=0;

	for (int i=1; i <= numberOfChromosomes; i++)
	{
		if (ploidy == 1)
		{
			chromTEcount = chromoVector.at(i-1).GetChromTECount();
			lossCount = (int)rand.Poisson(vt*chromTEcount);
			if (lossCount > chromTEcount)
			  lossCount = chromTEcount;

			for (int k=0; k < lossCount; k++)
			{
			  nthTE = (int)(rand.Uniform()*chromTEcount + 1);
			  chromoVector.at(i-1).Delete(nthTE);
			  chromTEcount--;
			}
		}
		if (ploidy == 2)
		{
		  for (int j=1; j <= ploidy; j++)
		  {
			chromTEcount = chromoVector.at(2*i+j-3).GetChromTECount();
			lossCount = (int)rand.Poisson(vt*chromTEcount);
			if (lossCount > chromTEcount)
			  lossCount = chromTEcount;

			for (int k=0; k < lossCount; k++)
			{
			  nthTE = (int)(rand.Uniform()*chromTEcount + 1);
			  chromoVector.at(2*i+j-3).Delete(nthTE);
			  chromTEcount--;
			}
		}
	  }
    }
}

void Ascus::Recombination()
{
	if (ploidy != 2)
	  return;

	Locus * current1 = 0;
	Locus * current2 = 0;
	Locus * next1 = 0;
	Locus * next2 = 0;
	double site = 0.0, r = 0.0;
	int events = 0, length = 0;

	std::vector<int> sitesVector;

	  for (int i=1; i <= numberOfChromosomes; i++)
	  {
	    // if no TE's, then recombination does nothing.
		if ((GetChromosome(i, 1).GetChromTECount() + GetChromosome(i, 2).GetChromTECount()) == 0)
		  return;

		// number of recombination events taken from a poisson distribution
		r = GetChromosome(i, 1).GetRecRate();
		length = (GetChromosome(i, 1).GetLength() - 1);
		events = (int)rand.Poisson(r*length);

		sitesVector.resize(events);

		for (int j=0; j < events; j++)
		{
			int pos = (int)((rand.Uniform())*(length) + 1);
			sitesVector.at(j) = pos;
		}

		std::sort(sitesVector.begin(), sitesVector.end());

		current1 = GetChromosome(i, 1).GetHeadLocus();
		current2 = GetChromosome(i, 2).GetHeadLocus();

		for(int a=0; a < events; a++)
		{
			site = sitesVector.at(a) + 0.5;
			//std::cout << site << std::endl;

			if ((current1 == 0) && (current2 == 0))
			  break;

			if (current1 != 0)
			  if (current1->GetTransposon().GetLocation() < site)
			  {
			    next1 = current1->GetNext();
				while(next1 != 0)
				{
				  if (next1->GetTransposon().GetLocation() > site)
					break;
				  current1 = next1;
				  next1 = current1->GetNext();
				}
			  }

			if (current2 != 0)
			  if (current2->GetTransposon().GetLocation() < site)
			  {
			    next2 = current2->GetNext();
				while(next2 != 0)
				{
				  if (next2->GetTransposon().GetLocation() > site)
					break;
				  current2 = next2;
				  next2 = current2->GetNext();
				}
			  }

			if ((current1 == 0) || (current2 == 0))
			{
			  if ((current1 == 0) && (current2->GetTransposon().GetLocation() > site))
				continue;

			  if ((current1 == 0) && (current2->GetTransposon().GetLocation() < site))
			  {
				current2->SetNext(current1);
				current1 = next2;
				GetChromosome(i, 1).SetHeadLocus(next2);
				continue;
			  }

			  if ((current2 == 0) && (current1->GetTransposon().GetLocation() > site))
				continue;

			  if ((current2 == 0) && (current1->GetTransposon().GetLocation() < site))
			  {
				current1->SetNext(current2);
				current2 = next1;
				GetChromosome(i, 2).SetHeadLocus(next1);
				continue;
			  }
			}

			if ((current1->GetTransposon().GetLocation() > site) && (current2->GetTransposon().GetLocation() > site))
			  continue;

			if ((current1->GetTransposon().GetLocation() > site) && (current2->GetTransposon().GetLocation() < site))
			{
			  current2->SetNext(current1);
			  current1 = next2;
			  GetChromosome(i, 1).SetHeadLocus(next2);
			  continue;
			}

			if ((current1->GetTransposon().GetLocation() < site) && (current2->GetTransposon().GetLocation() > site))
			{
			  current1->SetNext(current2);
			  current2 = next1;
			  GetChromosome(i, 2).SetHeadLocus(next1);
			  continue;
			}

			// both current1 & current2 < site

			if (next1 == 0 && next2 == 0)
			  break;
			else
			{
				current1->SetNext(next2);
				current2->SetNext(next1);
			}
		}
		//std::cout << "Recombination for chromo: " << i << std::endl;

		current1 = 0;
		current2 = 0;
		next1 = 0;
		next2 = 0;
		site = 0.0;
		events = 0;
		length = 0;
		r = 0;
		sitesVector.clear();

	  } // for: numberOfChromosome

} // Recombination method


//Ascus Ascus::MakeAscus()
//{
//
//	// for ploidy == 2
//
//	Recombination();
//	Ascus gamete(numberOfChromosomes, 1);
//
//	int c = 0;
//
//	for (int i=1; i <= numberOfChromosomes; i++)
//	{
//		if (rand.Uniform() < 0.5)
//		  c = 1;
//		else
//		  c = 2;
//
//		Locus * current = GetChromosome(i,c).GetHeadLocus();
//		while (current != 0)
//		{
//			gamete.GetChromosome(i,1).Insert(current->GetTransposon());
//			current = current->GetNext();
//		}
//	}
//
//	return gamete;
//}

void Ascus::ListAscusSites() const
{
	for (int i=0; i < (numberOfChromosomes*ploidy); i++)
	  chromoVector.at(i).ListChromSites();
	std::cout << std::endl;
}