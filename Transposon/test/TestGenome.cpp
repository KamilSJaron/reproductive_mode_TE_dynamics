// simulator Transposon
// testing class Locus

#include "../include/TestGenome.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestGenome);

using namespace CppUnit;
using namespace std;

void TestGenome::setUp(void) {
    Genome::SetParameters();
    ind1 = new Genome();
    ind2 = new Genome(4);
    // ind3 = new Genome( ind1 );
}

void TestGenome::tearDown(void) {
    delete ind1;
}

void TestGenome::testConstructor(void) {}