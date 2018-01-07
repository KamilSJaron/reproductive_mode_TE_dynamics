// simulator Transposon
// testing class Locus

#include "../include/TestPopulation.h"
#include "../include/genome.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestPopulation);

using namespace CppUnit;
using namespace std;

void TestPopulation::setUp(void) {
	pop = new Population(20);
	pop->Initialize();
}

void TestPopulation::tearDown(void) {
	delete pop;
}

void TestPopulation::testSelectVitalIndividual(void) {
	int selected_ind = 0;
	selected_ind = pop->SelectVitalIndividual();
	// prints a matrix 30x30 of the rolled numbers - just for quick visual inspection
	// std::cerr << "\n";
	// for(int i = 0; i < 30; i++){
	// 	for(int j = 0; j < 30; j++){
	// 		std::cerr << pop->SelectVitalIndividual() << " ";
	// 	}
	// 	std::cerr << "\n";
	// }
	CPPUNIT_ASSERT(selected_ind >= 0 && selected_ind <= 19);
}

void TestPopulation::testSexualReproduction(void) {
	Population * new_population;
	new_population = pop->SexualReproduction();
}

void TestPopulation::testAsexualReproduction(void) {
	Population * new_population;
	new_population = pop->AsexualReproduction();
}