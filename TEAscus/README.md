# TEAscus

Transposable element simulator of haploid yeast with sexual reproduction though ascus.

### Installation

```
git clone https://github.com/KamilSJaron/yeast_TE_load_prediction
cd yeast_TE_load_prediction/TEAscus
make
# make test
make install
```

### Usage

```
TEAscus [--version] [--help] [--sex]
```

where `--sex` cause a sexual generation every 90 generations. Few parameters are hardcoded (number of chromosomes, number of loci, recombination rates in `src/Genome.cpp`, number of generations, number of replicates and frequency of sex / output in `src/main.cpp`)

The rest of parameters are loaded from file `input.txt`

```
Line 1: N (population size)
Line 2: ut (transposition rate u)
Line 3: vt (excision rate v)
Line 4: sa (a, selection parameter)
Line 5: sb (b, synergism within family selection parameter)
Line 6: faf (frequency affecting fitness)
```

### Details of simulation

Simulator is created to reproduce experiment of [McDonald et al.](www.nature.com/doifinder/10.1038/nature17143) as closely as possible. Simulations run for *990 generations*, every 90 generations there is (or isn't) a sexual generation, this is also the generation when stats are captured (after reproduction).

 * 16 segregating Chromosomes
 * 500 slots (loci) for TEs on each chromosome (i.e. 8000 in total)
 * recombination rates of each chromosome are modeled as poisson process with 5.6 chiasmata according to [mapping of meiotic crossovers in yeast](dx.doi.org/10.1038/nature07135) totaling in ~90 crossovers per meiosis.

Asexual reproduction is clonal and perfect (no mutation). Sexual reproduction is reformed through fusion of two haploid individuals into **ascus** followed by meiosis, producing four new haploid individuals (hence the name of the simulator).

The selection is acting on haploid individuals, not on diploid asci. The population is of fixed size and the choice of individuals to next generation is implemented using Monte Carlo (just like in the original simulator).

### Testing

Unit tests are written using [CppUnit framework](https://wiki.freedesktop.org/www/Software/cppunit/) saved in `test/` and their headers are in `include`. Every class has one test class and the naming scheme is straightforward (just prefix `Test` before a class name).

To run test write command

```
make test
```

which automatically (re)compiles what is needed, compile testing program, run it and if it passes then also delete the testing program.

The tests were written and tested on OS X using `clang` compiler. Simple test on Ubuntu with CppUnit installed though `apt-get` have failed with non-trivial error. Pull-requests are welcomed on this issue.

### Origin of the code

The original code was provided by Elie Dolgin and it was used for publication "[The Fate of Transposable Elements in Asexual Populations](https://doi.org/10.1534/genetics.106.060434)"; DOI: 10.1534/genetics.106.060434
(the code as it was shared with me is accessible in commit [7daac13](https://github.com/KamilSJaron/yeast_TE_load_prediction/tree/7daac13fdccd4b5ce1d2c7f169c63a269f9442cc/Transposon)). The software was called simply Transposon and it have implemented asexual reproduction and sexual reproduction of diploid individuals.

The main modification is fixed haploid individuals with possible sexual and asexual reproduction mode.

I also added unit tests, and simplified code on several places.