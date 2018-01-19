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

#include "../include/Population.h"
#include "../include/Genome.h"
#include "../include/Random.h"
#include <fstream>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <algorithm>

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

double Population::GetMeanU() const {
	double mean_u = 0.0, genome_mean_u = 0.0;
	int genomes_with_TEs = 0;
	for (int i=0; i < popSize; i++) {
		genome_mean_u = GetIndividual(i).GetMeanU();
		if (genome_mean_u != -1){
			mean_u += genome_mean_u;
			genomes_with_TEs++;
		}
	}
	mean_u /= (double)genomes_with_TEs;
	return(mean_u);
}

void Population::Initialize() {
	int rolled_position = 0, rolled_chromosome = 0, rolled_position_on_ch = 0;
	int totalLength = Genome::chromLength * Genome::numberOfChromosomes;

	if (Genome::initialTE > totalLength) {
		std::cerr << "Error : more initial transposons is set than slots in the simulated genome. " << std::endl;
		std::cerr << "\tinitial transposons : " << Genome::initialTE << std::endl;
		std::cerr << "\tslots in the simulated genome : " << totalLength << std::endl;
		exit(EXIT_FAILURE);
	}

	/// this generates 10 initial TEs in a genome of individual 0
	for (int j=0; j < Genome::initialTE; j++) {
		// std::cerr << "Creating TE : " << j+1 << std::endl;
		do {
			Genome::GenerateChromosomeAndPosition(& rolled_chromosome, & rolled_position_on_ch);
		} while (!GetIndividual(0).GetChromosome(rolled_chromosome).TestEmpty(rolled_position_on_ch));

		GetIndividual(0).GetChromosome(rolled_chromosome).Insert(Transposon(rolled_position_on_ch, Genome::u_initial, true));
	}

	/// copy individual 0 to all other individuals
	for (int a=1; a < popSize; a++) {
		for (int i=1; i <= Genome::numberOfChromosomes; i++) {
			Locus * current = GetIndividual(0).GetChromosome(i).GetHeadLocus();

			while (current != 0) {
				GetIndividual(a).GetChromosome(i).Insert(current->GetTransposon());
				current = current->GetNext();
			}
		}
	}
}

int Population::SelectVitalIndividual() const{
	bool viable = false;
	double fitness = 0;
	int ind = -1;

	while (!viable){
		ind = (int)((rand.Uniform())*(popSize));
		fitness = GetIndividual(ind).GetGenomeFitness();
		if (rand.Uniform() < fitness)
			viable = true;
	}
	return ind;
}

void Population::DeleteIndividual(int x) {
	for (int i=1; i <= Genome::numberOfChromosomes; i++) {
		delete genoVector.at(x).GetChromosome(i).GetHeadLocus();
		genoVector.at(x).GetChromosome(i).SetHeadLocus(0);
	}
}

Population * Population::SexualReproduction() {
	Population * newPopulation = new Population(popSize);

	/// Every two selected parents will generate 4 ofsprings.
	for (int ind = 0; ind < popSize; ind += 4) {
		/// selecting parents for ind, ind+1, ind+2, ind+3
		Genome parent1(GetIndividual(SelectVitalIndividual()));
		Genome parent2(GetIndividual(SelectVitalIndividual()));

		/// every pair of parents generate 4 offsprings (2 and 2)
		generateTwoOspring(ind, newPopulation, parent1, parent2);
		generateTwoOspring(ind + 2, newPopulation, parent1, parent2);
	}

	return newPopulation;
}

Population * Population::AsexualReproduction() {
	Population * newPopulation = new Population(popSize);

	for (int a=0; a < popSize; a++) {
		// std::cerr << "Creating with ind " << a << " from pop of " << popSize << " individuals." << std::endl;
		/// taking selected individual : ind
		Genome parent(GetIndividual(SelectVitalIndividual()));

		/// not entiraly sure what this bit does
		for (int i=1; i <= Genome::numberOfChromosomes; i++) {
			Locus * current = parent.GetChromosome(i).GetHeadLocus();
			while (current != 0) {
				newPopulation->GetIndividual(a).GetChromosome(i).Insert(current->GetTransposon());
				current = current->GetNext();
			}
		}
	} // for

	return newPopulation;
}

void Population::TranspositionAndLoss()
{
	for (int i=0; i < popSize; i++) {
		genoVector.at(i).Transpose();
		genoVector.at(i).ElementLoss();
	}
}

void Population::ListPopulationSites() const {
	for (int i=0; i < popSize; i++) {
		std::cerr << "INDIVIDUAL [" << i << "]" << std::endl;
		genoVector.at(i).ListGenomeSites();
	}
}

void Population::SaveParameters(const char * fileName) {
	std::ofstream fout(fileName,std::ios::app);

	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	// OUTPUT TO FILE

	fout << "\n" << asctime (timeinfo) << "\n";
	fout << "N = " << popSize << "\n";
	fout << "u = " << Genome::u_initial << ", v = " << Genome::vt << "\n";
	fout << "initialTE = " << Genome::initialTE << "\n";
	fout << "propAffectW = " << Genome::faf << "\n";
	fout << "a = " << Genome::sa << ", b = " << Genome::sb << "\n";
	fout << "chrom# = " << Genome::numberOfChromosomes << ", ploidy# = haploid\n" << "\n";

	fout << "GEN" << "\t" << "n" << "\t" << "Vn" << "\t" << "x" << "\t" << "Vx" << "\t";
	fout << "empty" << "\t" << "fixed" << "\t" << "min#" << "\t" << "minFreq" << "\t" << "mean_u" << "\n";

	fout.close();
}

void Population::PrintParameters(){
	time_t rawtime;
	struct tm * timeinfo;
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	// OUTPUT TO SCREEN

	std::cerr << asctime (timeinfo) << std::endl;
	std::cerr << "Population Size = " << popSize << std::endl;
	std::cerr << "Transposition Rate = " << Genome::u_initial << std::endl;
	std::cerr << "Excision Rate = " << Genome::vt << std::endl;
	std::cerr << "Initial TE count = " << Genome::initialTE << std::endl;
	std::cerr << "Proportion affecting fitness = " << Genome::faf << std::endl;
	std::cerr << "Selection parameters, a = " << Genome::sa << ", b = " << Genome::sb << std::endl;
	std::cerr << "Number of chromosomes = " << Genome::numberOfChromosomes << ". Ploidy = haploid" << std::endl << std::endl;
}

// Statisitics for entire genome
void Population::SummaryStatistics(const char * fileName, int generation)
{
	std::ofstream fout(fileName,std::ios::app);

	// to determine mean and variance of copy number per individual
	double meanCopyNumber=0.0, varCopyNumber=0.0, x=0.0;
	double proportionAffectingW=0.0;
	double mean_u = 0.0; //, var_u = 0.0;
	int chromLength=0, vectorLength=0, y=0;

	int size = GetPopSize();

	int minCopyNum = 0; double minCopyFreq = 1.0;

	for (int i=1; i <= Genome::numberOfChromosomes; i++)
		vectorLength += GetIndividual(0).GetChromosome(i).GetLength();

	std::vector<int> locationVector(vectorLength, 0);

	meanCopyNumber = ((double)GetPopulationTECount()) / ((double)size);
	mean_u = GetMeanU();

	if (meanCopyNumber != 0)
		proportionAffectingW = ((double)GetPopulationTECountAffectingFitness()) / ((double)GetPopulationTECount());
	minCopyNum = (int)meanCopyNumber + 1;

	for (int i=0; i < size; i++) {
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

	for (int j=1; j <= Genome::numberOfChromosomes; j++) {
		chromLength = GetIndividual(0).GetChromosome(j).GetLength();
		for (int k=1; k <= chromLength; k++) {
			for (int m=0; m < size; m++) {
				empty1 = GetIndividual(m).GetChromosome(j).TestEmpty(k);
				empty2 = GetIndividual(m).GetChromosome(j).TestEmpty(k);

				if (!empty1 || !empty2)
					emptyTest = false;
				if (empty1 && empty2)
					fixedTest = false;
				if (!empty1) {
					locationVector.at(a)++;
					meanFreq++;
				}
				if (!empty2) {
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

	for (int i=0; i < vectorLength; i++) {
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
	std::cout << "Minimum copy frequency: " << minCopyFreq << std::endl;
	std::cout << "Mean transposable rate: " << mean_u << std::endl;
	std::cout << std::endl;
	// std::cout << "Variance in transposable rates: " << var_u << std::endl << std::endl;
	// OUTPUT TO FILE

	fout << generation << "\t" << meanCopyNumber << "\t" << varCopyNumber << "\t" << meanFreq << "\t";
	fout << varFreq << "\t" << fractionEmpty << "\t" << fractionFixed << "\t" << minCopyNum <<"\t";
	fout << minCopyFreq << "\t" << mean_u << "\n";

	fout.close();
}

void Population::RecordPopulation(const char * fileName, int generation)
{
	std::ofstream fout(fileName, std::ios::trunc);

	fout << generation << "\n";

	int size = GetPopSize();
	Locus * loc = GetIndividual(0).GetChromosome(1).GetHeadLocus();

	for (int i=0; i < size; i++) {
		if (GetIndividual(i).GetGenomeTECount()==0)
			fout << ".\n";
		else {
			for (int j=1; j <= Genome::numberOfChromosomes; j++) {
				loc = GetIndividual(i).GetChromosome(j).GetHeadLocus();
				if (loc != 0) {
					while (loc->GetNext() != 0) {
						fout << j << "\n" << loc->GetTransposon().GetLocation() << "\n";
						loc = loc->GetNext();
					}
					fout << j << "\n" << loc->GetTransposon().GetLocation() << "\n";
					if (j == Genome::numberOfChromosomes){
						fout << ".\n";
					}
				}
			} //for j
		} // else
	} //for i
}

// Statistics for one chromosome
void Population::SummaryStatistics(int num)
{
	// to determine mean and variance of copy number per individual
	double meanCopyNumber=0.0, varCopyNumber=0.0, x=0.0;
	int chromLength = GetIndividual(0).GetChromosome(num).GetLength();
	int size = GetPopSize();

	std::vector<int> countVector(size, 0);
	std::vector<int> locationVector(chromLength, 0);

	for (int i=0; i < size; i++)
	{
		countVector.at(i) = GetIndividual(i).GetChromosome(num).GetChromTECount()
							+ GetIndividual(i).GetChromosome(num).GetChromTECount();
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
			empty1 = GetIndividual(m).GetChromosome(num).TestEmpty(k);
			empty2 = GetIndividual(m).GetChromosome(num).TestEmpty(k);

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

	double r = GetIndividual(0).GetChromosome(num).GetRecRate();

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
		vectorLength += GetIndividual(0).GetChromosome(i).GetLength();

	std::vector<double> meanVector(numberOfChromosomesAnalyzed, 0);
	std::vector<double> varVector(numberOfChromosomesAnalyzed, 0);
	std::vector<int> countVector(size, 0);

	for (int j=0; j < numberOfChromosomesAnalyzed; j++)
	{
		for (int i=0; i < size; i++)
		{
		countVector.at(i) = GetIndividual(i).GetChromosome(j+numFirst).GetChromTECount()
							+ GetIndividual(i).GetChromosome(j+numFirst).GetChromTECount();
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
		chromLength = GetIndividual(0).GetChromosome(j+numFirst).GetLength();
		std::vector<int> locationVector(chromLength, 0);

		for (int k=1; k <= chromLength; k++)
		{
		for (int m=0; m < size; m++)
		{
			empty1 = GetIndividual(m).GetChromosome(j+numFirst).TestEmpty(k);
			empty2 = GetIndividual(m).GetChromosome(j+numFirst).TestEmpty(k);

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
		x = (double)locationVector.at(i) / (2*size) -	meanVector.at(j);
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

///////////////
/// PRIVATE ///
///////////////

int Population::getLocusPosition(Locus * loc) const {
	if (loc == 0)
		return 0; /// i.e. chromosome is TEless
	else
		return loc->GetPosition();
}

void Population::generateTwoOspring(int ind,
									Population * newPopulation,
									Genome & parent1, Genome & parent2){

	Locus *loc_par1, *loc_par2;
	int pos1 = 0, pos2 = 0;
	std::vector<int> chiasmas;
	int chiasma = 0, num_of_chiasmas = 0;
	int last_roll = -1;
	bool crossing = Genome::GenerateTossACoin();
	/// crossing == true --> 	offspring1 -\/- parent2 &
	///							offspring2 -/\- parent1
	/// crossing == false --> 	offspring1 ---- parent1 &
	///							offspring2 ---- parent2

	/// every chromosome will be rocombined separatedly
	for (int ch = 1; ch <= Genome::numberOfChromosomes; ch++) {

		/// Roll chiasmas for positions and sort them
		num_of_chiasmas = Genome::GenerateNumberOfChiasmas(ch);
		for(int chiasma_i = 0; chiasma_i < num_of_chiasmas; chiasma_i++){
			chiasmas.push_back( Genome::GenerateGapPositionOnChromosome() );
		}
		sort(chiasmas.begin(), chiasmas.end());

		/// kick out position rolled twice
		for(int chiasma_i = 0; chiasma_i < num_of_chiasmas; chiasma_i++){
			if(last_roll == chiasmas[chiasma_i]){
				chiasmas.erase (chiasmas.begin() + chiasma_i, chiasmas.begin() + chiasma_i + 1);
				chiasma_i -= 2;
				num_of_chiasmas -= 2;
				last_roll = -1;
			} else {
				last_roll = chiasmas[chiasma_i];
			}
		}

		/// load head loci of chromosomes ch of selected parents
		loc_par1 = parent1.GetChromosome(ch).GetHeadLocus();
		loc_par2 = parent2.GetChromosome(ch).GetHeadLocus();
		/// get their location of the chromosome
		pos1 = getLocusPosition(loc_par1);
		pos2 = getLocusPosition(loc_par2);

		/// for each chiasma write parental TEs to appropriate offpring determined by variable crossing
		for (int chiasma_i = 0; chiasma_i < num_of_chiasmas; chiasma_i++){
			chiasma = chiasmas[chiasma_i]; // load apropriate chiasma

			/// TODO resolve how to make 4, not 2 gametes
			/// (perhaps a function that will take a reference to new population and basal index to fill)
			if (crossing) {
				/// write parent1 to offspring ind
				while(pos1 < chiasma and pos1 != 0){
					newPopulation->GetIndividual(ind).GetChromosome(ch).Insert(loc_par1->GetTransposon());
					loc_par1 = loc_par1->GetNext();
					pos1 = getLocusPosition(loc_par1);
				}
				/// write parent2 to offspring ind + 1
				while(pos2 < chiasma and pos2 != 0){
					newPopulation->GetIndividual(ind+1).GetChromosome(ch).Insert(loc_par2->GetTransposon());
					loc_par2 = loc_par2->GetNext();
					pos2 = getLocusPosition(loc_par2);
				}
			} else {
				/// write parent1 to offspring ind + 1
				while(pos1 < chiasma and pos1 != 0){
					newPopulation->GetIndividual(ind+1).GetChromosome(ch).Insert(loc_par1->GetTransposon());
					loc_par1 = loc_par1->GetNext();
					pos1 = getLocusPosition(loc_par1);
				}
				/// write parent2 to offspring ind
				while(pos2 < chiasma and pos2 != 0){
					newPopulation->GetIndividual(ind).GetChromosome(ch).Insert(loc_par2->GetTransposon());
					loc_par2 = loc_par2->GetNext();
					pos2 = getLocusPosition(loc_par2);
				}
			}

			crossing = !crossing;
		}

		if (crossing) {
			/// parent1 to offspring ind
			while(pos1 != 0){
				newPopulation->GetIndividual(ind).GetChromosome(ch).Insert(loc_par1->GetTransposon());
				loc_par1 = loc_par1->GetNext();
				pos1 = getLocusPosition(loc_par1);
			}
			/// parent2 to offspring ind + 1
			while(pos2 != 0){
				newPopulation->GetIndividual(ind+1).GetChromosome(ch).Insert(loc_par2->GetTransposon());
				loc_par2 = loc_par2->GetNext();
				pos2 = getLocusPosition(loc_par2);
			}
		} else {
			/// parent1 to offspring ind + 1
			while(pos1 != 0){
				newPopulation->GetIndividual(ind+1).GetChromosome(ch).Insert(loc_par1->GetTransposon());
				loc_par1 = loc_par1->GetNext();
				pos1 = getLocusPosition(loc_par1);
			}
			/// parent2 to offspring ind
			while(pos2 != 0){
				newPopulation->GetIndividual(ind).GetChromosome(ch).Insert(loc_par2->GetTransposon());
				loc_par2 = loc_par2->GetNext();
				pos2 = getLocusPosition(loc_par2);
			}
		}
	}
}
