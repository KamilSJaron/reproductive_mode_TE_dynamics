#include "population.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

#include "random.h"
#include "time.h"

using namespace std;

int main ()
{
  cerr << endl;
  const char *input_file = "input.txt";
  const char *detailed_out = "detailed.txt";
  const char *summary_out = "summary.txt";

  for (int run=1; run<=10; run++)
  {


    cerr << "Run : " << run << endl;
    std::ifstream fin(input_file);
        if (! fin.is_open())
          { cerr << "Error opening file"; exit (1); }

        // Initialize population size & whether to generate new population or load from file
        int N;

        /// fromFile=true is caling a code that reads stuff from a file that is missing
        /// code was commented and warning message was added
        bool fromFile = false;
        char tempChar[100];
        fin.getline(tempChar,100);
        N=strtol(tempChar,0,10);
    fin.close();

    Population * pop = new Population(N);
    Population * temp;

    int size = pop->GetPopSize();
    bool clonal = Genome::clonal;

    pop->Initialize(clonal, fromFile);
    pop->PrintParameters(detailed_out);

    if (run==1) pop->PrintParameters(summary_out);
    pop->SummaryStatistics(detailed_out, 0);

    for (int gen = 1; gen <= 1000; gen++)
    {
        if (pop->GetPopulationTECount() == 0)
        {
          cerr << "No TEs at generation [" << gen << "]." << endl << endl;
          pop->SummaryStatistics(summary_out, gen);
          pop->SummaryStatistics(detailed_out, gen);
          break;
        }

        if (((double)pop->GetPopulationTECount()/(double)size) > 150.0)
        {
          cerr << "Population extinction at generation [" << gen << "]." << endl << endl;
          pop->SummaryStatistics(summary_out, gen);
          pop->SummaryStatistics(detailed_out, gen);
          break;
        }

        // REPRODUCTION
        temp = pop->AsexualReproduction();
        delete pop;
        pop = temp;

        // TRANSPOSITION & LOSS
        pop->TranspositionAndLoss();

        /// printing results
        cerr << ".";
        if (gen % 90 == 0)
        {
          cerr << endl;
          pop->SummaryStatistics(detailed_out, gen);
        }
    }

    delete pop;

  }

  return 0;
}
