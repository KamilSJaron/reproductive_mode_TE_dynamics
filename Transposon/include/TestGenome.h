#ifndef GENOMETEST_H
#define GENOMETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../include/Genome.h"

using namespace std;

class TestGenome : public CPPUNIT_NS :: TestFixture {
	CPPUNIT_TEST_SUITE (TestGenome);
	CPPUNIT_TEST (testConstructor);
	CPPUNIT_TEST (testLocusIteration);
	CPPUNIT_TEST (testRandomness);
	CPPUNIT_TEST_SUITE_END ();

	public:
		void setUp (void);
		void tearDown (void);

	protected:
		void testConstructor (void);
		void testLocusIteration (void);
		void testRandomness (void);

	private:
		Genome *ind1, *ind2, *ind3;
};

#endif
