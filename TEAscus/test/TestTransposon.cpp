// simulator Transposon
// testing class Transposon

// #include "../include/Transposon.h"
#include "../include/TestTransposon.h"

CPPUNIT_TEST_SUITE_REGISTRATION (TestTransposon);

using namespace CppUnit;
using namespace std;

void TestTransposon::setUp(void) {
	transposon = new Transposon(230, 0.003, true);
}

void TestTransposon::tearDown(void) {
	delete transposon;
}

void TestTransposon::testGetter(void) {
	CPPUNIT_ASSERT_EQUAL(transposon->GetLocation(), 230);
}