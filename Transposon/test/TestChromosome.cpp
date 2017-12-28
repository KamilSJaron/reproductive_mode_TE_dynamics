// simulator Transposon
// testing class Locus

// #include "../include/Transposon.h"
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