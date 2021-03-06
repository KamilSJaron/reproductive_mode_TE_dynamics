# FIGURES = figures/sim_asex_dipl_2ch.png figures/sim_asex_dipl.png figures/sim_sex.png figures/sim_asex.png
FIGURES := $(patsubst sims/%, figures/%.png,$(wildcard sims/0*)) figures/asex_sim_sex_equllibrum.png figures/asex_sex_naive.png

MEIOTIC_TRANSP_INPUTS := $(wildcard sims/02_higher_meiotic_transposition_rates/*/input.txt)
MODIFIER_INPUTS := $(wildcard sims/03_general_modifier_of_exision_rates/*/input.txt)

DOLGIN_SIMS := sims/01_Dolgin_and_Charlesworth_model/sex_detailed.txt sims/01_Dolgin_and_Charlesworth_model/asex_detailed.txt
MEIOTIC_TRANSP_SIMS := $(patsubst %/input.txt, %/sex_detailed.txt, $(MEIOTIC_TRANSP_INPUTS)) $(patsubst %/input.txt, %/asex_detailed.txt, $(MEIOTIC_TRANSP_INPUTS))
MODIFIER_SIMS := $(patsubst %/input.txt, %/sex_detailed.txt, $(MODIFIER_INPUTS)) $(patsubst %/input.txt, %/asex_detailed.txt, $(MODIFIER_INPUTS))

# if not defined, do 10 replicates
ifndef REP
REP := 10
endif


## make help	: print this help
.PHONY : help
help : Makefile
	@sed -n 's/^##//p' $<

## make empirical	: print how empirical data were analyzed (in markdown)
.PHONY : empirical
empirical :
	@echo "All the scripts are in the directory empirics\n"
	@cat empirics/README.md


# .PHONY : all
# all : presentation/asex_TE_accumulation.pdf

## make sims	: run all the simulations (~50 simulations)
.PHONY : sims
sims : $(DOLGIN_SIMS) $(MEIOTIC_TRANSP_SIMS) $(MODIFIER_SIMS)

## make rebuild	: rebuild the simulator
.PHONY : rebuild
rebuild :
	cd TEAscus && $(MAKE)

# .PHONY : clean
# clean :
# 	rm $(FIGURES) presentation/asex_TE_accumulation.pdf

# presentation/asex_TE_accumulation.pdf : $(FIGURES) presentation/asex_TE_accumulation.tex
# 	cd presentation && pdflatex asex_TE_accumulation
#
# figures/%.png : scripts/plot_simulation.R sims/%/detailed.txt
# 	Rscript $^ $@
#
# figures/asex_sim_sex_equllibrum.png : scripts/plot_asex_sim_sex_equllibrium.R
# 	Rscript $<
#
# figures/asex_sex_naive.png : scripts/plot_asex_sex_naive_sim.R
# 	Rscript $<

.PRECIOUS : sims/%/sex_detailed.txt
sims/%/sex_detailed.txt : TEAscus/TEAscus sims/%/input.txt
	cd sims/$* && ../../$< -r $(REP) -s -o sex_detailed.txt 1> sex_TEs_per_decade_2.tsv 2> sex_log.err

.PRECIOUS : sims/%/asex_detailed.txt
sims/%/asex_detailed.txt : TEAscus/TEAscus sims/%/input.txt
	cd sims/$* && ../../$< -r $(REP) -o asex_detailed.txt 1> asex_TEs_per_decade_2.tsv 2> asex_log.err


# build only if it does not exist, otherwise use what is there
TEAscus/TEAscus :
	cd TEAscus && $(MAKE)
