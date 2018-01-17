// simulator Transposon
// testing class Locus

#include "../include/Transposon.h"
#include "../include/TestChromosome.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestChromosome);

using namespace CppUnit;
using namespace std;

void TestChromosome::setUp(void) {
	ch1 = new Chromosome();
	ch2 = new Chromosome(1,1,10,1.53);
}

void TestChromosome::tearDown(void) {
	// delete locus1;
	delete ch1;
}

void TestChromosome::testConstructor(void) {}

void TestChromosome::testTEcount(void) {
	int num_of_chromosomes;

	/// expect zero
	num_of_chromosomes = ch1->GetChromTECount();
	CPPUNIT_ASSERT_EQUAL(num_of_chromosomes, 0);
	/// make a toy transposon and insert it in chromosome
	ch1->Insert(Transposon(230, 0.005, true));
	/// check that reported GetChromTECOunt is 1
	num_of_chromosomes = ch1->GetChromTECount();
	CPPUNIT_ASSERT_EQUAL(num_of_chromosomes, 1);
	/// insert another one
	ch1->Insert(Transposon(69, 0.005, false));
	num_of_chromosomes = ch1->GetChromTECount();
	CPPUNIT_ASSERT_EQUAL(num_of_chromosomes, 2);
	/// check TE
	num_of_chromosomes = ch1->GetChromTECountAffectingFitness();
	CPPUNIT_ASSERT_EQUAL(num_of_chromosomes, 1);
}

void TestChromosome::testTestEmpty(void) {
	bool is_empty;

	is_empty = ch1->TestEmpty(0);
	CPPUNIT_ASSERT_EQUAL(is_empty, true);

	ch1->Insert(Transposon(3, 0.005, true));
	CPPUNIT_ASSERT_EQUAL(ch1->TestEmpty(2), true);
	CPPUNIT_ASSERT_EQUAL(ch1->TestEmpty(3), false);
	CPPUNIT_ASSERT_EQUAL(ch1->TestEmpty(4), true);
}