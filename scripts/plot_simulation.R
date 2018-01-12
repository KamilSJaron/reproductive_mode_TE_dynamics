# 1. sim
# 2. output

args <- commandArgs(trailingOnly = TRUE)

source('scripts/read_simulation.R')

# 'sims/001_basic_diploid_ch2/detailed.txt'
sim <- read_simulation( args[1] )

TEs <- unlist(sapply(sim, function(x){ x$n }))

# 'figures/sim_2ch_literature_pars.png'
png( args[2] )

plot(NULL,
    xlim = c(1, 990), ylim = c(min(TEs), max(TEs)),
    xlab = 'Generation', ylab = 'number of TEs',
    main = 'simulated TEs')

all_plots <- sapply(sim, function(x){ lines(x$n ~ x$GEN) })

dev.off()