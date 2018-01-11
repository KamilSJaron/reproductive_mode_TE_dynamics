
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <cstring>

#include "time.h"

#include "../include/Population.h"
#include "../include/Random.h"
#include "../include/gitversion.h"

#define REPLICATES 10
#define NUMBER_OF_GENERATIONS 990
#define REPORT_AND_SEX_PERIOD 90

using namespace std;

int main(int argc, char **argv){

	/* constants - input and output files */
	const char *detailed_out = "detailed.txt";
	const char *input_file = "input.txt";
	bool sex = false;
	string runnig_sex = "False";

	cerr << "Running Transposon" << endl;
	/* start of dev vesion */
	cerr << "Commit: " << GITVERSION << endl;
	/* end */
	cerr << "Input : " << input_file << endl;

	if(argc > 1){
			if (strcmp( argv[1], "--help") == 0 or strcmp( argv[1], "-h") == 0) {
					cout << "most of parameters are read form an input file input.txt" << endl;
					cout << "details are in readme file" << endl;
					cout << "\t TEAscus [--version] [--help] [--sex]" << endl;

					return 0;
			}
			if (strcmp( argv[1], "--version") == 0 or strcmp( argv[1], "-v") == 0) {
					cout << "TEAscus fork of Transposon ( doi : 10.1534%2Fgenetics.106.060434 )" << endl;
					cout << "\thttps://github.com/KamilSJaron/yeast_TE_load_prediction/tree/master/TEAscus" << endl;
					cout << "\tcommit: " << GITVERSION << endl;
					return 0;
			}
			if (strcmp( argv[1], "--sex") == 0){
					sex = true;
					runnig_sex = "True";
			}
	}

	cerr << "Sex every " << REPORT_AND_SEX_PERIOD << " generations : " << runnig_sex << endl;

	for (int run=1; run <= REPLICATES; run++) {
		cerr << "Run : " << run << endl;
		std::ifstream fin(input_file);
			if (! fin.is_open())
				{ cerr << "Error opening file : " << input_file << endl; exit (1); }

			// Initialize population size & whether to generate new population or load from file
			int N;
			char tempChar[100];
			fin.getline(tempChar,100);
			N=strtol(tempChar,0,10);
		fin.close();

		Population * pop = new Population(N);
		// cerr << "Population created" << endl;
		Population * temp;

		int size = pop->GetPopSize();
		// cerr << "Population size : " << size << endl;
		pop->Initialize();
		// cerr << "Population initiated" << endl;
		pop->SaveParameters(detailed_out);

		if (run==1) {
			pop->PrintParameters();
		}
		pop->SummaryStatistics(detailed_out, 0);

		for (int gen = 1; gen <= NUMBER_OF_GENERATIONS; gen++) {
			// cerr << "Running generation " << gen << "." << endl;

			if (pop->GetPopulationTECount() == 0 or ((double)pop->GetPopulationTECount()/(double)size) > 150.0) {
				// cerr << "No TEs at generation [" << gen << "]." << endl << endl;
				// cerr << "Population extinction at generation [" << gen << "]." << endl << endl;
				pop->SummaryStatistics(detailed_out, gen);
				break;
			}

			// cerr << "Reproducing " << endl;
			// REPRODUCTION
			if ( gen % REPORT_AND_SEX_PERIOD == 0 and sex ){
				temp = pop->SexualReproduction();
			} else {
				temp = pop->AsexualReproduction();
			}

			delete pop;
			pop = temp;

			// cerr << "Transposing " << endl;
			// TRANSPOSITION & LOSS
			pop->TranspositionAndLoss();

			/// printing results after transposition
			cerr << ".";
			if (gen % REPORT_AND_SEX_PERIOD == 0) {
				cerr << endl;
				pop->SummaryStatistics(detailed_out, gen);
			}
		}

		delete pop;

	}

	return 0;
}
