// simulator Transposon
// testing class Locus

#include "../include/Transposon.h"
#include "../include/TestLocus.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestLocus);

using namespace CppUnit;
using namespace std;

void TestLocus::setUp(void) {
	locus1 = new Locus(Transposon(7, 0.007, true));
	locus2 = new Locus(Transposon(4, 0.008, true), locus1);
	locus3 = new Locus(Transposon());
}

void TestLocus::tearDown(void) {
	delete locus2; /// destructor of Locus destroys whole chain of loci
	delete locus3;
}

void TestLocus::testGetPosition(void) {
	int pos = locus1->GetPosition();
	CPPUNIT_ASSERT_EQUAL(7, pos);
	pos = locus2->GetPosition();
	CPPUNIT_ASSERT_EQUAL(4, pos);
	pos = locus3->GetPosition();
	CPPUNIT_ASSERT_EQUAL(0, pos);
}