# FIGURES = figures/sim_asex_dipl_2ch.png figures/sim_asex_dipl.png figures/sim_sex.png figures/sim_asex.png
FIGURES := $(patsubst sims/%, figures/%.png,$(wildcard sims/0*))

.PHONY : all
all : presentation

.PHONY : clean
clean :
	rm $(FIGURES) presentation/asex_TE_accumulation.pdf

.PHONY : presentation
presentation : presentation/asex_TE_accumulation.pdf

presentation/asex_TE_accumulation.pdf : $(FIGURES) presentation/asex_TE_accumulation.tex
	cd presentation && pdflatex asex_TE_accumulation

figures/%.png : scripts/plot_simulation.R sims/%/detailed.txt
	Rscript $^ $@
