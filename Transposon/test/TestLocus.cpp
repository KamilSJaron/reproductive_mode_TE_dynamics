// simulator Transposon
// testing class Locus

#include "../include/Transposon.h"
#include "../include/TestLocus.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestLocus);

using namespace CppUnit;
using namespace std;

void TestLocus::setUp(void) {
    locus1 = new Locus(Transposon(7, true));
    locus2 = new Locus(Transposon(4, true), locus1);
}

void TestLocus::tearDown(void) {
    // delete locus1;
    delete locus2;
}

void TestLocus::testConstructor(void) {}