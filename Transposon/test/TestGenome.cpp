// simulator Transposon
// testing class Locus

#include "../include/TestGenome.h"
#include "../include/Locus.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestGenome);

using namespace CppUnit;
using namespace std;

void TestGenome::setUp(void) {
	Genome::SetParameters();
	ind1 = new Genome();
	// ind2 = new Genome( ind1 );
}

void TestGenome::tearDown(void) {
	delete ind1;
}

void TestGenome::testConstructor(void) {
	int static_ch = Genome::numberOfChromosomes;
	int size_of_ind1_ch_vector = ind1->GetEmpiricalNumberOfChromosomes();
	// int size_of_ind2_ch_vector = ind2->GetEmpiricalNumberOfChromosomes();

	CPPUNIT_ASSERT_EQUAL(static_ch, size_of_ind1_ch_vector);
	// CPPUNIT_ASSERT_EQUAL(static_ch, size_of_ind2_ch_vector);
}

void TestGenome::testLocusIteration(void){
	Locus * current;
	for (int i=1; i <= Genome::numberOfChromosomes; i++) {
		current = ind1->GetChromosome(i).GetHeadLocus();

		while (current != 0) {
			current = current->GetNext();
		}
	} // for
}

void TestGenome::testRandomness(void){
	std::vector<int> generated_chiasmas;
	std::vector<int> generated_positions;
	int generated_number = -1;

	for (int ch = 1; ch <= 1; ch++){ //Genome::numberOfChromosomes
		for (int i = 0; i < 10000; i++){
			generated_chiasmas.push_back(Genome::GenerateNumberOfChiasmas(ch));
		}
	}

	int zeros = 0;
	int genome_lengths = 0;

	for (int i = 0; i < 10000; i++){
		generated_number = Genome::GenerateGapPositionOnChromosome();
		// if (generated_number == 0){
		// 	zeros++;
		// }
		// if(generated_number == Genome::chromLength){
		// 	genome_lengths++;
		// }
		// generated_positions.push_back(generated_number);
	}

	// std::cerr << "\n" << "Proportion of zeros : " << (double)zeros / 10000 << "\n";
	// std::cerr << "Proportion of max : " << (double)genome_lengths / 10000 << "\n";
	// std::cerr << "expected proportion : " << (double)1 / (Genome::chromLength + 1) << "\n";
}