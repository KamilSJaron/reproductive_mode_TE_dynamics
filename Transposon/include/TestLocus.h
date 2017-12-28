// CppUnit-Tutorial
// file: fractiontest.h
#ifndef LOCUSTEST_H
#define LOCUSTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../include/Locus.h"

using namespace std;

class TestLocus : public CPPUNIT_NS :: TestFixture {
	CPPUNIT_TEST_SUITE (TestLocus);
	CPPUNIT_TEST (testConstructor);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void testConstructor (void);

	private:
		Locus *locus1, *locus2;
};

#endif
