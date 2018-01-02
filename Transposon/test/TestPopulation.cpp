// simulator Transposon
// testing class Locus

#include "../include/TestPopulation.h"
#include "../include/genome.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestPopulation);

using namespace CppUnit;
using namespace std;

void TestPopulation::setUp(void) {
	Genome::SetParameters();
	pop = new Population(10);
}

void TestPopulation::tearDown(void) {
	delete pop;
}

void TestPopulation::testSelectVitalIndividual(void) {
	int selected_ind = 0;
	selected_ind = pop->SelectVitalIndividual();
	CPPUNIT_ASSERT(selected_ind >= 0 && selected_ind <= 11);
}