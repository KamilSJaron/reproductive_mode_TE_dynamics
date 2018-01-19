#ifndef TRANSPOSONTEST_H
#define TRANSPOSONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../include/Transposon.h"

using namespace std;

class TestTransposon : public CPPUNIT_NS :: TestFixture {
	CPPUNIT_TEST_SUITE (TestTransposon);
	CPPUNIT_TEST (testGetLocation);
	CPPUNIT_TEST (testTransposonRate);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void testGetLocation (void);
		void testTransposonRate (void);

	private:
		Transposon *transposon;
};

#endif
