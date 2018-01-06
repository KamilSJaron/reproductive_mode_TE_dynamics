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