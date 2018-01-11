.PHONY : all
all : figures/sim_asex_dipl_2ch.png figures/sim_asex_dipl.png figures/sim_sex.png figures/sim_asex.png figures/sim_sex_inc_tr.png

figures/sim_asex_dipl_2ch.png : scripts/plot_simulation.R sims/001_basic_diploid_ch2/detailed.txt
	Rscript $^ $@

figures/sim_asex_dipl.png : scripts/plot_simulation.R sims/002_basic_diploid_ch16/detailed.txt
	Rscript $^ $@

figures/sim_sex.png : scripts/plot_simulation.R sims/003_basic_sex/detailed.txt
	Rscript $^ $@

figures/sim_asex.png : scripts/plot_simulation.R sims/004_basic_asex/detailed.txt
	Rscript $^ $@

# increased transposition rate
figures/sim_sex_inc_tr.png : scripts/plot_simulation.R sims/005_increase_transposition_rate_sex/detailed.txt
	Rscript $^ $@

