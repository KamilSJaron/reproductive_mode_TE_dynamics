#include "population.h"
#include <iostream>
#include <fstream>
#include <vector>

#include "random.h"
#include "time.h"

using namespace std;

int main ()
{
  cerr << endl;
  for (int run=1; run<=10; run++) {
    cerr << "Run : " << run << endl;
    std::ifstream fin("input.txt");
    if (! fin.is_open())
      { cout << "Error opening file"; exit (1); }

    // Initialize population size & whether to generate new population or load from file
    int N;
    bool fromFile = true;
    char tempChar[100];
    fin.getline(tempChar,100);
    if (tempChar[0]=='Y') fromFile = true;
    else if (tempChar[0]=='N') fromFile = false;
    fin.getline(tempChar,100);
    N=strtol(tempChar,0,10);
    fin.close();

    cerr << "Initializing population : " << N << endl;
    Population * pop = new Population(N);
    Population * temp;

    int size = pop->GetPopSize();
    bool clonal = Genome::clonal;

    pop->Initialize(clonal, fromFile);

    pop->PrintParameters("detailed.txt");
    if (run==1) pop->PrintParameters("summary.txt");
    pop->SummaryStatistics("detailed.txt", 0);
    int gen = 1;

    for (gen; gen <= 100000; gen++)
    {
        if (pop->GetPopulationTECount() == 0)
        {
          cout << "No TEs at generation [" << gen << "]." << endl << endl;
          pop->SummaryStatistics("summary.txt", gen);
          pop->SummaryStatistics("detailed.txt", gen);
          break;
        }

        if (((double)pop->GetPopulationTECount()/(double)size) > 150.0)
        {
          cout << "Population extinction at generation [" << gen << "]." << endl << endl;
          pop->SummaryStatistics("summary.txt", gen);
          pop->SummaryStatistics("detailed.txt", gen);
          break;
        }

        // REPRODUCTION
        temp = pop->AsexualReproduction();
        delete pop;
        pop = temp;

        // TRANSPOSITION & LOSS
        pop->TranspositionAndLoss();

        cout << ".";
        if (gen%100==0)
        {
          cout << endl;
          pop->SummaryStatistics("detailed.txt", gen);
        }
//        if (gen%1000==0)
//        {
//          pop->RecordPopulation("population.txt", gen);
//        }
    }

    delete pop;

  }

  return 0;
}
