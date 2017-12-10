// *********************************************************************
//
// population.cpp
//
// Created by: Elie Dolgin, University of Edinburgh
//
// First started: March 17, 2005
// Last edited:
//
// *********************************************************************

#include "population.h"
#include "genome.h"
#include <fstream>
#include <iostream>
#include <time.h>

Random Population::rand;

Population::Population(int size)
{
	popSize = size;
	genoVector.resize(popSize);
}

Population::~Population()
{}

Genome & Population::GetIndividual(int x)
{
	return genoVector.at(x);
}

const Genome & Population::GetIndividual(int x) const
{
	return genoVector.at(x);
}

int Population::GetPopSize() const
{
	return popSize;
}

unsigned int Population::GetPopulationTECount() const
{
	unsigned int populationTEcount = 0;
	for (int i=0; i < popSize; i++)
	  populationTEcount += GetIndividual(i).GetGenomeTECount();
	return populationTEcount;
}

unsigned int Population::GetPopulationTECountAffectingFitness() const
{
	unsigned int populationTEcount = 0;
	for (int i=0; i < popSize; i++)
	  populationTEcount += GetIndividual(i).GetGenomeTECountAffectingFitness();
	return populationTEcount;
}

double Population::GetPopulationMeanFitness() const
{
	double populationFitness = 0.0;
	for (int i=0; i < popSize; i++)
	  populationFitness += GetIndividual(i).GetGenomeFitness();
	populationFitness /= (double)popSize;
	return populationFitness;
}

void Population::Initialize(bool clonal, bool fromFile)
{
	int individualNumberOfTEs=0, num=0, copy=0, pos=0, totalLength=0, currentLength=0;
	int numberOfChromosomes = Genome::GetNumberOfChromosomes();
	int genomePloidy = Genome::GetPloidy();
	int n = Genome::initialTE;

	for (int i=1; i <= numberOfChromosomes; i++)
	  totalLength += GetIndividual(0).GetChromosome(i, 1).GetLength();

	if (fromFile)
	{
		std::cerr << "This doesn't work (do something else, fromFile is True).\n";
	  // std::ifstream fin("population.txt");
	  // if (! fin.is_open())
	  // { std::cout << "Error opening file of population details"; exit (1); }
      //
	  // char tempChar[5];
	  // // NEED TO FIRST HAVE GENERATION
	  // for (int a=0; a < popSize; a++)
	  // {
	  //   while(!fin.getline(tempChar, 5).eof())
	  //   {
		//   fin.getline(tempChar,5);
		//   while (tempChar != ".")
		//   {
		// 	num=strtol(tempChar,0,5);
		// 	fin.getline(tempChar,5);
		// 	copy=strtol(tempChar,0,5);
		// 	fin.getline(tempChar,5);
		// 	pos=strtol(tempChar,0,5);
      //
		// 	GetIndividual(a).GetChromosome(num,copy).Insert(Transposon(pos, true));
      //
		// 	fin.getline(tempChar,5);
		//   }
		// }
	  // }

	  return;
	}

	if (clonal)
	{
	  individualNumberOfTEs = n;
	  if (individualNumberOfTEs > 2*totalLength)
		individualNumberOfTEs = 2*totalLength;

	  for (int j=0; j < individualNumberOfTEs; j++)
	  {
		do {
			pos = (int)((rand.Uniform()*totalLength) + 1);
			num = 1;
			for (int k=1; k <= numberOfChromosomes; k++)
			{
			  currentLength = GetIndividual(0).GetChromosome(k, 1).GetLength();
			  if (pos > currentLength)
			  {
				num++;
				pos -= currentLength;
			  }
			  else
				break;
			}
			copy = (int)((rand.Uniform())*(genomePloidy) + 1);
		} while (!GetIndividual(0).GetChromosome(num, copy).TestEmpty(pos));

		GetIndividual(0).GetChromosome(num,copy).Insert(Transposon(pos, true));
	  }

	  for (int a=1; a < popSize; a++)
	  {
		for (int i=1; i <= numberOfChromosomes; i++)
		{
		  for (int j=1; j<= genomePloidy; j++)
		  {
			Locus * current = GetIndividual(0).GetChromosome(i,j).GetHeadLocus();

			while (current != 0)
			{
			  GetIndividual(a).GetChromosome(i,j).Insert(current->GetTransposon());
			  current = current->GetNext();
			}
		  }
		}
	  }
	}

	else // not clonal
	{
	  double fractionAffectingW = Genome::GetFAF();
	  bool affectW = false;

	  for (int i=0; i < popSize; i++)
	  {
		individualNumberOfTEs = (int)rand.Poisson(n);

		if (individualNumberOfTEs > 2*totalLength)
		  individualNumberOfTEs = 2*totalLength;

		for (int j=0; j < individualNumberOfTEs; j++)
		{
			do {
			  pos = (int)((rand.Uniform()*totalLength) + 1);
			  num = 1;
			  for (int k=1; k <= numberOfChromosomes; k++)
			  {
				currentLength = GetIndividual(i).GetChromosome(k, 1).GetLength();
				if (pos > currentLength)
				{
				  num++;
				  pos -= currentLength;
				}
				else
				  break;
			  }
			  copy = (int)((rand.Uniform())*(genomePloidy) + 1);
			} while (!GetIndividual(i).GetChromosome(num, copy).TestEmpty(pos));


			if (fractionAffectingW > rand.Uniform())
			  affectW = true;
			else
			  affectW = false;

			GetIndividual(i).GetChromosome(num,copy).Insert(Transposon(pos, affectW));
		}
	  }
	}
}

Genome Population::MakeIndividual()
{
	// for ploidy == 2

	Genome newIndividual;

	int ind = 0;
	int c = 0;
	int p = 1;
	double fitness = 0.0;
	int chr = Genome::GetNumberOfChromosomes();

	while (p <= 2)
	{
	  ind = (int)((rand.Uniform())*(popSize));
	  Genome parent(GetIndividual(ind));
	  fitness = parent.GetGenomeFitness();

	  if (rand.Uniform() < fitness)
		{
		  parent.Recombination();

		  for (int i=1; i <= chr; i++)
		  {
			if (rand.Uniform() < 0.5)
			  c = 1;
			else
			  c = 2;

			//std::cout << "Strand #" << c << " inherited." << std::endl;

			Locus * current = parent.GetChromosome(i,c).GetHeadLocus();

			while (current != 0)
			{
				newIndividual.GetChromosome(i,p).Insert(current->GetTransposon());
				current = current->GetNext();
			}
		  }
		  p++;
		}
	}
	return newIndividual;
}

void Population::DeleteIndividual(int x)
{
	int chromNumber = Genome::GetNumberOfChromosomes();
	int ploidy = Genome::GetPloidy();

	for (int i=1; i <= chromNumber; i++)
	  for (int j=1; j <= ploidy; j++)
	  {
		delete genoVector.at(x).GetChromosome(i,j).GetHeadLocus();
		genoVector.at(x).GetChromosome(i,j).SetHeadLocus(0);
	  }
}

Population * Population::SexualReproduction()
{
	Population * newPopulation = new Population(popSize);

	int ind = 0, c = 0, p = 0;
	bool viable = false;
	double fitness = 0.0;
	int chr = Genome::GetNumberOfChromosomes();

	for (int a=0; a < popSize; a++)
	{
	  viable = false;
	  while (!viable)
	  {
		p=1;

		while (p <= 2)
		{
		  ind = (int)((rand.Uniform())*(popSize));
		  Genome parent(GetIndividual(ind));

		  parent.Recombination();

		  for (int i=1; i <= chr; i++)
		  {
			if (rand.Uniform() < 0.5)
			  c = 1;
			else
			  c = 2;

			Locus * current = parent.GetChromosome(i,c).GetHeadLocus();

			while (current != 0)
			{
			  newPopulation->GetIndividual(a).GetChromosome(i,p).Insert(current->GetTransposon());
			  current = current->GetNext();
			}
		  } // for

		  p++;
		} // while (p <= 2)

		fitness = newPopulation->GetIndividual(a).GetGenomeFitness();

		if (rand.Uniform() < fitness)
		  viable = true;
		else
		{
		  newPopulation->DeleteIndividual(a);
		  viable = false;
		}

	  } // while (!viable)
	} // for

	return newPopulation;
}

Population * Population::AsexualReproduction()
{
	Population * newPopulation = new Population(popSize);

	int ind = 0;
	bool viable = false;
	double fitness = 0.0;
	int chr = Genome::GetNumberOfChromosomes();
	int pl = Genome::GetPloidy();

	for (int a=0; a < popSize; a++)
	{
	  viable = false;
	  do
	  {
		ind = (int)((rand.Uniform())*(popSize));
		fitness = GetIndividual(ind).GetGenomeFitness();

		if (rand.Uniform() < fitness)
		  viable = true;
		else
		  viable = false;

	  }while (!viable);

		Genome parent(GetIndividual(ind));

		for (int i=1; i <= chr; i++)
		{
		  for (int j=1; j<= pl; j++)
		  {
			Locus * current = parent.GetChromosome(i,j).GetHeadLocus();

			while (current != 0)
			{
			  newPopulation->GetIndividual(a).GetChromosome(i,j).Insert(current->GetTransposon());
			  current = current->GetNext();
			}
		  }
		}
	} // for

	return newPopulation;
}

// fitnessBased: true if bottleneck based on fitness of surviving individuals, false if random
Population * Population::Bottleneck(int bottleneckSize, bool fitnessBased, bool sexual)
{
  Population * newPopulation = new Population(popSize);

  int ind = 0, vectorPosition = 0;
  bool viable = false;
  double fitness = 0.0;
  int chr = Genome::GetNumberOfChromosomes();
  std::vector<int>bottleneckVector(bottleneckSize,0);

  for (int i=0; i < bottleneckSize; i++)
  {
	if (fitnessBased)
	{
	  do
	  {
		ind = (int)((rand.Uniform())*(popSize));
		fitness = GetIndividual(ind).GetGenomeFitness();

		if (rand.Uniform() < fitness)
		  viable = true;
		else
		  viable = false;

	  }while (!viable);
	}
	else
	{
	  ind = (int)((rand.Uniform())*(popSize));
    }

    bottleneckVector.at(i) = ind;
  }

  if (sexual)
  {
	int c=0, p=0;
	for (int a=0; a < popSize; a++)
	{
	  viable = false;
	  while (!viable)
	  {
		p=1;

		while (p <= 2)
		{
		  vectorPosition = (int)((rand.Uniform())*(bottleneckSize));
		  ind = bottleneckVector.at(vectorPosition);
		  Genome parent(GetIndividual(ind));

		  parent.Recombination();

		  for (int i=1; i <= chr; i++)
		  {
			if (rand.Uniform() < 0.5)
			  c = 1;
			else
			  c = 2;

			Locus * current = parent.GetChromosome(i,c).GetHeadLocus();

			while (current != 0)
			{
			  newPopulation->GetIndividual(a).GetChromosome(i,p).Insert(current->GetTransposon());
			  current = current->GetNext();
			}
		  } // for

		  p++;
		} // while (p <= 2)

		fitness = newPopulation->GetIndividual(a).GetGenomeFitness();

		if (rand.Uniform() < fitness)
		  viable = true;
		else
		{
		  newPopulation->DeleteIndividual(a);
		  viable = false;
		}

	  } // while (!viable)
	} // for
  } // if

  else
  {
	int pl = Genome::GetPloidy();

	for (int a=0; a < popSize; a++)
	{
	  viable = false;
	  do
	  {
		vectorPosition = (int)((rand.Uniform())*(bottleneckSize));
		ind = bottleneckVector.at(vectorPosition);
		fitness = GetIndividual(ind).GetGenomeFitness();

		if (rand.Uniform() < fitness)
		  viable = true;
		else
		  viable = false;

	  }while (!viable);

		Genome parent(GetIndividual(ind));

		for (int i=1; i <= chr; i++)
		{
		  for (int j=1; j<= pl; j++)
		  {
			Locus * current = parent.GetChromosome(i,j).GetHeadLocus();

			while (current != 0)
			{
			  newPopulation->GetIndividual(a).GetChromosome(i,j).Insert(current->GetTransposon());
			  current = current->GetNext();
			}
		  }
		}
	} // for
  } // else

  std::cout << std::endl << "Population bottleneck of size[" << bottleneckSize << "]" << std::endl;
  return newPopulation;
}


void Population::TranspositionAndLoss()
{
	for (int i=0; i < popSize; i++)
	{
		genoVector.at(i).Transpose();
		genoVector.at(i).ElementLoss();
	}
}

void Population::ListPopulationSites() const
{
	for (int i=0; i < popSize; i++)
	{
	  std::cout << "INDIVIDUAL [" << i << "]" << std::endl;
	  genoVector.at(i).ListGenomeSites();
	}
}

void Population::PrintParameters(const char * fileName)
{
	std::ofstream fout(fileName,std::ios::app);

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	// OUTPUT TO SCREEN

	std::cout << asctime (timeinfo) << std::endl;
	std::cout << "Population Size = " << popSize << std::endl;
	std::cout << "Transposition Rate = " << Genome::ut << std::endl;
	std::cout << "Excision Rate = " << Genome::vt << std::endl;
	std::cout << "Initial TE count = " << Genome::initialTE << std::endl;
	std::cout << "Proportion affecting fitness = " << Genome::faf << std::endl;
	std::cout << "Selection parameters, a = " << Genome::sa << ", b = " << Genome::sb << std::endl;
	std::cout << "Number of chromosomes = " << Genome::numberOfChromosomes << ". Ploidy = " << Genome::ploidy << std::endl << std::endl;

	// OUTPUT TO FILE

	fout << "\n" << asctime (timeinfo) << "\n";
	fout << "N = " << popSize << "\n";
	fout << "u = " << Genome::ut << ", v = " << Genome::vt << "\n";
	fout << "initialTE = " << Genome::initialTE << "\n";
	fout << "propAffectW = " << Genome::faf << "\n";
	fout << "a = " << Genome::sa << ", b = " << Genome::sb << "\n";
	fout << "chrom# = " << Genome::numberOfChromosomes << ", ploidy# = " << Genome::ploidy << "\n" << "\n";

	fout << "GEN" << "\t" << "n" << "\t" << "Vn" << "\t" << "x" << "\t" << "Vx" << "\t";
	fout << "empty" << "\t" << "fixed" << "\t" << "min#" << "\t" << "minFreq" << "\n";

	fout.close();
}

// Statisitics for entire genome
void Population::SummaryStatistics(const char * fileName, int generation)
{
	std::ofstream fout(fileName,std::ios::app);

	// to determine mean and variance of copy number per individual
	double meanCopyNumber=0.0, varCopyNumber=0.0, x=0.0;
	double proportionAffectingW=0.0;
	int chromLength=0, vectorLength=0, y=0;

	int chromNumber = Genome::GetNumberOfChromosomes();
	int size = GetPopSize();

	int minCopyNum = 0; double minCopyFreq = 1.0;

	for (int i=1; i <= chromNumber; i++)
		vectorLength += GetIndividual(0).GetChromosome(i,1).GetLength();

	std::vector<int> locationVector(vectorLength, 0);

	meanCopyNumber = ((double)GetPopulationTECount()) / ((double)size);
	if (meanCopyNumber != 0)
	  proportionAffectingW = ((double)GetPopulationTECountAffectingFitness()) / ((double)GetPopulationTECount());
	minCopyNum = (int)meanCopyNumber + 1;

	for (int i=0; i < size; i++)
	{
		y = GetIndividual(i).GetGenomeTECount();
		x = y - meanCopyNumber;
		varCopyNumber += x*x;

		if (y == minCopyNum)
			minCopyFreq++;
		if (y < minCopyNum)
		{
			minCopyNum = y;
			minCopyFreq = 1.0;
		}
	}
	varCopyNumber /= ((double)(size - 1));
	minCopyFreq /= ((double)size);

	// to determine number of empty sites and variation between sites.
	int a=0;
	int emptySites=0, fixedSites=0;
	bool empty1, empty2, emptyTest = true, fixedTest = true;
	double meanFreq = 0.0, varFreq = 0.0, fractionEmpty=0.0, fractionFixed=0.0;

	for (int j=1; j <= chromNumber; j++)
	{
		chromLength = GetIndividual(0).GetChromosome(j,1).GetLength();
		for (int k=1; k <= chromLength; k++)
		{
			for (int m=0; m < size; m++)
			{
				empty1 = GetIndividual(m).GetChromosome(j,1).TestEmpty(k);
				empty2 = GetIndividual(m).GetChromosome(j,2).TestEmpty(k);

				if (!empty1 || !empty2)
				  emptyTest = false;
				if (empty1 && empty2)
				  fixedTest = false;

				if (!empty1)
				{
				  locationVector.at(a)++;
				  meanFreq++;
				}
				if (!empty2)
				{
				  locationVector.at(a)++;
				  meanFreq++;
				}
			}
			if (emptyTest)
			  emptySites++;
			if (fixedTest)
			  fixedSites++;
			emptyTest = true;
			fixedTest = true;
			a++;
		}
	}
	meanFreq /= (2*a*size);

	for (int i=0; i < vectorLength; i++)
	{
		x = (double)locationVector.at(i) / (2*size) - meanFreq;
		varFreq += x*x;
	}
	varFreq /= (a - 1);

	fractionEmpty = (double)emptySites / (double)vectorLength;
	fractionFixed = (double)fixedSites / (double)vectorLength;

	// OUTPUT TO SCREEN
	std::cout << "GENERATION [" << generation << "]" << std::endl;
	std::cout << "Mean copy number per individual: " << meanCopyNumber << std::endl;
	std::cout << "Variance in copy number between individuals: " << varCopyNumber << std::endl;
	std::cout << "Proportion affecting fitness: ";
	  if (meanCopyNumber == 0)
		std::cout << "n/a" << std::endl;
	  else
		std::cout << proportionAffectingW << std::endl;
	std::cout << "Mean element frequency: " << meanFreq << std::endl;
	std::cout << "Variance in element frequency: " << varFreq << std::endl;
	std::cout << "Fraction of loci with zero frequency: " << fractionEmpty << std::endl;
	std::cout << "Fraction of fixed loci: " << fractionFixed << std::endl;
	std::cout << "Minimum copy number: " << minCopyNum << std::endl;
	std::cout << "Minimum copy frequency: " << minCopyFreq << std::endl << std::endl;

	// OUTPUT TO FILE

	fout << generation << "\t" << meanCopyNumber << "\t" << varCopyNumber << "\t" << meanFreq << "\t";
	fout << varFreq << "\t" << fractionEmpty << "\t" << fractionFixed << "\t" << minCopyNum <<"\t";
	fout << minCopyFreq << "\n";

	fout.close();
}

void Population::RecordPopulation(const char * fileName, int generation)
{
	std::ofstream fout(fileName, std::ios::trunc);

	fout << generation << "\n";

	int chromNumber = Genome::GetNumberOfChromosomes();
	int size = GetPopSize();
	int ploidy = Genome::GetPloidy();
	Locus * loc = GetIndividual(0).GetChromosome(1,1).GetHeadLocus();

	for (int i=0; i < size; i++)
	{
	  if (GetIndividual(i).GetGenomeTECount()==0)
		fout << ".\n";
	  else
	  {
		for (int j=1; j <= chromNumber; j++)
		{
			for (int k=1; k <= ploidy; k++)
			{
				loc = GetIndividual(i).GetChromosome(j,k).GetHeadLocus();

				if (loc != 0)
				{
				  while (loc->GetNext() != 0)
				  {
					fout << j << "\n" << k << "\n" << loc->GetTransposon().GetLocation() << "\n";
					loc = loc->GetNext();
				  }
				  if (j==chromNumber && k==ploidy)
				    fout << j << "\n" << k << "\n" << loc->GetTransposon().GetLocation() << "\n.\n";
				  else
				    fout << j << "\n" << k << "\n" << loc->GetTransposon().GetLocation() << "\n";
				}
			} //for k
		} //for j
	  } // else
	} //for i
}

// Statistics for one chromosome
void Population::SummaryStatistics(int num)
{
	// to determine mean and variance of copy number per individual
	double meanCopyNumber=0.0, varCopyNumber=0.0, x=0.0;
	int chromLength = GetIndividual(0).GetChromosome(num,1).GetLength();
	int size = GetPopSize();

	std::vector<int> countVector(size, 0);
	std::vector<int> locationVector(chromLength, 0);

	for (int i=0; i < size; i++)
	{
		countVector.at(i) = GetIndividual(i).GetChromosome(num,1).GetChromTECount()
							+ GetIndividual(i).GetChromosome(num,2).GetChromTECount();
		meanCopyNumber += countVector.at(i);
	}

	meanCopyNumber /= (2*size);

	for (int i=0; i < size; i++)
	{
		x = countVector.at(i) - meanCopyNumber;
		varCopyNumber += x*x;
	}
	varCopyNumber /= (size - 1);

	// to determine number of empty sites and variation between sites.
	int emptySites=0, fixedSites=0;
	bool empty1, empty2, emptyTest = true, fixedTest = true;
	double meanFreq = 0.0, varFreq = 0.0, fractionEmpty=0.0, fractionFixed=0.0;

	for (int k=1; k <= chromLength; k++)
	{
		for (int m=0; m < size; m++)
		{
			empty1 = GetIndividual(m).GetChromosome(num,1).TestEmpty(k);
			empty2 = GetIndividual(m).GetChromosome(num,2).TestEmpty(k);

			if (!empty1 || !empty2)
				emptyTest = false;
			if (empty1 || empty2)
				fixedTest = false;

			if (!empty1)
			{
				locationVector.at(k-1)++;
				meanFreq++;
			}
			if (!empty2)
			{
				locationVector.at(k-1)++;
				meanFreq++;
			}
		}
		if (emptyTest)
		  emptySites++;
		if (fixedTest)
		  fixedSites++;
		emptyTest = true;
		fixedTest = true;
	}
	meanFreq /= (2*chromLength*size);

	for (int i=0; i < chromLength; i++)
	{
		x = (double)locationVector.at(i) / (2*size) - meanFreq;
		varFreq += x*x;
	}
	varFreq /= (chromLength - 1);

	fractionEmpty = (double)emptySites / (double)chromLength;
	fractionFixed = (double)fixedSites / (double)chromLength;

	double r = GetIndividual(0).GetChromosome(num,1).GetRecRate();

	std::cout << "CHROMOSOME [" << num << "]: Size = " << chromLength << ". r = " << r << std::endl;
	std::cout << "Mean copy number per chromosome: " << meanCopyNumber << std::endl;
	std::cout << "Variance in copy number between chromosomes: " << varCopyNumber << std::endl;
	std::cout << "Mean element frequency: " << meanFreq << std::endl;
	std::cout << "Variance in element frequency: " << varFreq << std::endl;
	std::cout << "Fraction of loci with zero frequency: " << fractionEmpty << std::endl;
	std::cout << "Fraction of fixed loci: " << fractionFixed << std::endl << std::endl;
}

// Statistics for multiple chromosomes between numFirst & numLast (inclusive)
void Population::SummaryStatistics(int numFirst, int numLast)
{
	// to determine mean and variance of copy number per individual
	double x=0.0, meanCopyNumber=0.0, varCopyNumber=0.0;
	int vectorLength=0, chromLength=0;
	int size = GetPopSize();
	int numberOfChromosomesAnalyzed = numLast-numFirst+1;

	for (int i=numFirst; i <= numLast; i++)
		vectorLength += GetIndividual(0).GetChromosome(i,1).GetLength();

	std::vector<double> meanVector(numberOfChromosomesAnalyzed, 0);
	std::vector<double> varVector(numberOfChromosomesAnalyzed, 0);
	std::vector<int> countVector(size, 0);

	for (int j=0; j < numberOfChromosomesAnalyzed; j++)
	{
	  for (int i=0; i < size; i++)
	  {
		countVector.at(i) = GetIndividual(i).GetChromosome(j+numFirst,1).GetChromTECount()
							+ GetIndividual(i).GetChromosome(j+numFirst,2).GetChromTECount();
		meanVector.at(j) += countVector.at(i);
	  }

	  meanVector.at(j) /= (2*size);

	  for (int i=0; i < size; i++)
	  {
		x = countVector.at(i) - meanVector.at(j);
		varVector.at(j) += x*x;
	  }
	  varVector.at(j) /= (size - 1);
	}

	for (int j=0; j < numberOfChromosomesAnalyzed; j++)
	{
	  meanCopyNumber += meanVector.at(j);
	  varCopyNumber += varVector.at(j);
	  meanVector.at(j) = 0;
	  varVector.at(j) = 0;
	}
	meanCopyNumber /= numberOfChromosomesAnalyzed;
	varCopyNumber /= numberOfChromosomesAnalyzed;

	// to determine number of empty sites and variation between sites.
	int emptySites=0, fixedSites=0;
	bool empty1, empty2, emptyTest = true, fixedTest = true;
	double meanFreq = 0.0, varFreq = 0.0, fractionEmpty = 0.0, fractionFixed = 0.0;
	std::vector<double> fractionEmptyVector(numberOfChromosomesAnalyzed,0.0);
	std::vector<double> fractionFixedVector(numberOfChromosomesAnalyzed,0.0);

	for (int j=0; j < numberOfChromosomesAnalyzed; j++)
	{
	  chromLength = GetIndividual(0).GetChromosome(j+numFirst,1).GetLength();
	  std::vector<int> locationVector(chromLength, 0);

	  for (int k=1; k <= chromLength; k++)
	  {
		for (int m=0; m < size; m++)
		{
			empty1 = GetIndividual(m).GetChromosome(j+numFirst,1).TestEmpty(k);
			empty2 = GetIndividual(m).GetChromosome(j+numFirst,2).TestEmpty(k);

			if (!empty1 || !empty2)
				emptyTest = false;
			if (empty1 || empty2)
				fixedTest = false;

			if (!empty1)
			{
				locationVector.at(k-1)++;
				meanVector.at(j)++;
			}
			if (!empty2)
			{
				locationVector.at(k-1)++;
				meanVector.at(j)++;
			}
		}
		if (emptyTest)
		  emptySites++;
		if (fixedTest)
		  fixedSites++;
		emptyTest = true;
		fixedTest = true;
	  }
	  meanVector.at(j) /= (2*chromLength*size);

	  for (int i=0; i < chromLength; i++)
	  {
		x = (double)locationVector.at(i) / (2*size) -  meanVector.at(j);
		varVector.at(j) += x*x;
	  }
	  varVector.at(j) /= (chromLength - 1);

	  fractionEmptyVector.at(j) = (double)emptySites / (double)chromLength;
	  emptySites = 0;
	  fractionFixedVector.at(j) = (double)fixedSites / (double)chromLength;
	  fixedSites = 0;
	}
	for (int j=0; j < numberOfChromosomesAnalyzed; j++)
	{
	  meanFreq += meanVector.at(j);
	  varFreq += varVector.at(j);
	  fractionEmpty += fractionEmptyVector.at(j);
	  fractionFixed += fractionFixedVector.at(j);
	}
	meanFreq /= numberOfChromosomesAnalyzed;
	varFreq /= numberOfChromosomesAnalyzed;
	fractionEmpty /= numberOfChromosomesAnalyzed;
	fractionFixed /= numberOfChromosomesAnalyzed;


	std::cout << "CHROMOSOMES [" << numFirst << "-" << numLast << "]" << std::endl;
	std::cout << "Mean copy number per chromosome: " << meanCopyNumber << std::endl;
	std::cout << "Variance in copy number between chromosomes: " << varCopyNumber << std::endl;
	std::cout << "Mean element frequency: " << meanFreq << std::endl;
	std::cout << "Variance in element frequency: " << varFreq << std::endl;
	std::cout << "Fraction of loci with zero frequency: " << fractionEmpty << std::endl;
	std::cout << "Fraction of fixed loci: " << fractionFixed << std::endl << std::endl;
}