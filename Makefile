# FIGURES = figures/sim_asex_dipl_2ch.png figures/sim_asex_dipl.png figures/sim_sex.png figures/sim_asex.png
FIGURES := $(patsubst sims/%, figures/%.png,$(wildcard sims/00*))

.PHONY : all
all : $(FIGURES)

.PHONY : clean
clean :
	rm $(FIGURES)

figures/%.png : scripts/plot_simulation.R sims/%/detailed.txt
	Rscript $^ $@