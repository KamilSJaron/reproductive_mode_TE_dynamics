# 1. sim
# 2. output

args <- commandArgs(trailingOnly = TRUE)

source('scripts/read_simulation.R')

# 'sims/003_basic_sex/detailed.txt'
sim <- read_simulation( args[1] )

TEs <- unlist(sapply(sim, function(x){ x$n }))

if( grepl('asex', args[1]) ){
    col <- "#92C5DECD"
} else {
    col <- "#D6604DED"
}

# 'figures/sim_2ch_literature_pars.png'
png( args[2] )

plot(NULL,
    xlim = c(1, max(unlist(sapply(sim, function(x){ x$GEN })))), ylim = c(min(TEs), max(TEs)),
    xlab = 'Generation', ylab = 'number of TEs',
    main = 'simulated TEs')

all_plots <- sapply(sim, function(x){ lines(x$n ~ x$GEN, col = col, lwd = 1.4) })

dev.off()