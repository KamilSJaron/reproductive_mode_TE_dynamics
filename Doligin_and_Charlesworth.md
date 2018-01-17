# The Fate of Transposable Elements in Asexual Populations

Assumptions :

- Diploid asexual lineage arising from an ancestral sexual population that contains deleterious TEs originating from a single ancestor.
- Elements are transmitted only vertically from parent to offspring.
- The ancestral copy number of TEs is `x`.
- The copy number may increase due to transposition, at a rate `u` per element, or decrease due to excision at a rate `v` per element modeled using Poisson process with parameter `u` and `v` respectively.

## Analytical model

- infinite asexual population

## Stochastic simulation model

- for population size investigation, assuming a constant population size `N`
- genome is made of 2 chromosomes and each of them is able to carry 200 TEs.
- new TEs insert is placed to random unoccupied place in genome
- fitness `w` of individual with `n` elements is : `w_n = exp( - an - 1/2 bn^2)` where `a` and `b` are constant selection coefficient

### parameters

#### desired

- genome is 2 chromosome diploid, 200 loci loci each
- transposition rate (u) : `u = 1e-4`
- excision rates (v) : `v = 1e-5` or `v = 1e-6`
- selection on segregating elements `1e-5 - 1e-4` -> `a = 1e-5` and `b` is calculated so epression

`n_eq = (sqrt(2*(a^2) + (u - v)^2) - 2*a) / 2b`

equals to initial number of transposons.

#### scaled down

for computational reasons. Demonstrated that does not matter (maybe we should re-do the demonstration).