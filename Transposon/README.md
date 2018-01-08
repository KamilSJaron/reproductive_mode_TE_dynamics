This code was provided by Elie Dolgin. It was originally used for publication "The Fate of Transposable Elements in Asexual Populations"; DOI: 10.1534/genetics.106.060434

The main modification is fixed haploid individuals with possible sexual and asexual reproduction mode. Asexual reproduction is clonal. Sexual reproduction is reformed through fusion of two haploid individuals into ascus followed by meiosis, producing four new haploid individuals.

The main reason was simplification of code that have worked for both haploid and diploid individuals, but to get as close to the yeast experiment we needed a combination of both.

The class `Genome` was turned to fixed haploid individual.
A new class `Ascus` was created to facilitate sexual reproduction.

The selection is acting on haploid individuals, not on diploid asci. The population is of fixed size and the choice of individuals to next generation is implemented using Monte Carlo (just like in the original simulator).