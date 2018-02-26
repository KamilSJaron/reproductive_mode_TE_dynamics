# 1. sim
# 2. output

# args <- commandArgs(trailingOnly = TRUE)

source('scripts/read_simulation.R')

# 'sims/003_basic_sex/detailed.txt'
sex_sim <- read_simulation( "sims/013_sex_equil/detailed.txt" )
asex_sim <- read_simulation( "sims/015_seq_equil_asex_sim/detailed.txt" )

TEs <- unlist(sapply(c(sex_sim, asex_sim), function(x){ x$n }))

asex_blue <- "#92C5DECD"
sex_red <- "#D6604DED"

# 'figures/sim_2ch_literature_pars.png'
png( "figures/asex_sim_sex_equllibrum.png" )

plot(NULL,
    xlim = c(1, max(unlist(sapply(sex_sim, function(x){ x$GEN })))),
	ylim = c(min(TEs), max(TEs) + 1),
    xlab = 'Generation', ylab = 'mean number of TEs',
    main = 'Asexual simulation using parameters of sexual equilibrium', bty = 'n')

all_plots <- sapply(sex_sim, function(x){ lines(x$n ~ x$GEN, col = sex_red, lwd = 1.4) })
all_plots <- sapply(asex_sim, function(x){ lines(x$n ~ x$GEN, col = asex_blue, lwd = 1.4) })

legend('topleft', title = 'sex',
       c('yes', 'no'), pch = 20, col = c(sex_red, asex_blue),
       bty = 'n')

dev.off()