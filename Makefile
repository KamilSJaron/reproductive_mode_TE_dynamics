# FIGURES = figures/sim_asex_dipl_2ch.png figures/sim_asex_dipl.png figures/sim_sex.png figures/sim_asex.png
FIGURES := $(patsubst sims/%, figures/%.png,$(wildcard sims/0*)) figures/asex_sim_sex_equllibrum.png figures/asex_sex_naive.png

.PHONY : all
all : presentation/asex_TE_accumulation.pdf

.PHONY : rebuild
rebuild :
	cd TEAscus && $(MAKE)

.PHONY : clean
clean :
	rm $(FIGURES) presentation/asex_TE_accumulation.pdf

presentation/asex_TE_accumulation.pdf : $(FIGURES) presentation/asex_TE_accumulation.tex
	cd presentation && pdflatex asex_TE_accumulation

figures/%.png : scripts/plot_simulation.R sims/%/detailed.txt
	Rscript $^ $@

figures/asex_sim_sex_equllibrum.png : scripts/plot_asex_sim_sex_equllibrium.R
	Rscript $<

figures/asex_sex_naive.png : scripts/plot_asex_sex_naive_sim.R
	Rscript $<

.PRECIOUS : sims/%/detailed.txt
sims/%/detailed.txt : TEAscus/TEAscus sims/%/input.txt
	cd sims/$* && ../../$< 2> stderr

# build only if it does not exist, otherwise use what is there
TEAscus/TEAscus :
	cd TEAscus && $(MAKE)
