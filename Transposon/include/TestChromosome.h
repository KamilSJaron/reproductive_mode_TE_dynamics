// CppUnit-Tutorial
// file: fractiontest.h
#ifndef CHROMTEST_H
#define CHROMTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../include/Chromosome.h"

using namespace std;

class TestChromosome : public CPPUNIT_NS :: TestFixture {
	CPPUNIT_TEST_SUITE (TestChromosome);
	CPPUNIT_TEST (testConstructor);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void testConstructor (void);

	private:
		Chromosome *ch1, *ch2;
};

#endif
