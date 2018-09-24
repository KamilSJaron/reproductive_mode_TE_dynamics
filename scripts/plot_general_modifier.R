# script for work in progress, this is just for the case that some of it would be usefull latter on

library(RColorBrewer)
source('scripts/read_simulation.R')
source('scripts/read_input.R')

sims_dirs <- dir('sims', pattern = "^2")
sims_files <- paste('sims', sims_dirs, rep(c('sex_TEs_per_decade_2.tsv','asex_TEs_per_decade_2.tsv'), each = 4), sep = '/')
input_files <- paste('sims', sims_dirs, 'input.txt', sep = '/')

asex_blue <- "#92C5DECD"
sex_red <- "#D6604DED"

pal <- c(sex_red, asex_blue)

sim_inputs <- lapply(input_files, read_input)
sims <- lapply(sims_files, read_TEs_per_decade)

pdf('figures/general_modifier_f0.01.pdf', width = 12, height = 6)

par(mfrow=c(1,2), oma=c(0,0,1.5,0), mar = c(5, 4, 1, 2) + 0.1)

plot(c(0, 1000), c(50,50),
     xlim = c(1, 990),
     ylim = c(1, 60), type = 'l',
     xlab = 'Generation', ylab = 'Mean number of TEs',
     bty = 'n', cex.axis = 1.4, cex.lab = 1.4, cex.main = 1.6)

sapply(sims[[2]], function(x){ lines(x$n ~ x$GEN, col = sex_red, lwd = 1.4, lty = 2) })
sapply(sims[[6]], function(x){ lines(x$n ~ x$GEN, col = asex_blue, lwd = 1.4, lty = 2) })

sapply(sims[[4]], function(x){ lines(x$n ~ x$GEN, col = sex_red, lwd = 1.4) })
sapply(sims[[8]], function(x){ lines(x$n ~ x$GEN, col = asex_blue, lwd = 1.4) })

legend('bottomleft', c('no epistasis', 'epistasis','sex', 'asex'),
       bty = 'n', pch = c(NA, NA, 20, 20), lty = c(1, 2, NA, NA),
       cex = 1.4, col = c(1,1,pal))

plot(NULL,
     xlim = c(1, 990),
     ylim = c(0, 1),
     xlab = 'Generation', ylab = 'frequency of the modifier in population',
     bty = 'n', cex.axis = 1.4, cex.lab = 1.4, cex.main = 1.6)

sapply(sims[[2]], function(x){ lines(x$f ~ x$GEN, col = sex_red, lwd = 1.4, lty = 2) })
sapply(sims[[6]], function(x){ lines(x$f ~ x$GEN, col = asex_blue, lwd = 1.4, lty = 2) })

sapply(sims[[4]], function(x){ lines(x$f ~ x$GEN, col = sex_red, lwd = 1.4) })
sapply(sims[[8]], function(x){ lines(x$f ~ x$GEN, col = asex_blue, lwd = 1.4) })

title("modifier initial freq 0.01", outer=TRUE)

dev.off()

pdf('figures/general_modifier_f0.1.pdf', width = 12, height = 6)

par(mfrow=c(1,2), oma=c(0,0,1.5,0), mar = c(5, 4, 1, 2) + 0.1)

plot(c(0, 1000), c(50,50),
     xlim = c(1, 990),
     ylim = c(1, 60), type = 'l',
     xlab = 'Generation', ylab = 'Mean number of TEs',
     bty = 'n', cex.axis = 1.4, cex.lab = 1.4, cex.main = 1.6)

sapply(sims[[1]], function(x){ lines(x$n ~ x$GEN, col = sex_red, lwd = 1.4, lty = 2) })
sapply(sims[[5]], function(x){ lines(x$n ~ x$GEN, col = asex_blue, lwd = 1.4, lty = 2) })

sapply(sims[[3]], function(x){ lines(x$n ~ x$GEN, col = sex_red, lwd = 1.4) })
sapply(sims[[7]], function(x){ lines(x$n ~ x$GEN, col = asex_blue, lwd = 1.4) })

legend('bottomleft', c('no epistasis', 'epistasis','sex', 'asex'),
       bty = 'n', pch = c(NA, NA, 20, 20), lty = c(1, 2, NA, NA),
       cex = 1.4, col = c(1,1,pal))

plot(NULL,
     xlim = c(1, 990),
     ylim = c(0, 1),
     xlab = 'Generation', ylab = 'frequency of the modifier in population',
     bty = 'n', cex.axis = 1.4, cex.lab = 1.4, cex.main = 1.6)

sapply(sims[[1]], function(x){ lines(x$f ~ x$GEN, col = sex_red, lwd = 1.4, lty = 2) })
sapply(sims[[5]], function(x){ lines(x$f ~ x$GEN, col = asex_blue, lwd = 1.4, lty = 2) })

sapply(sims[[3]], function(x){ lines(x$f ~ x$GEN, col = sex_red, lwd = 1.4) })
sapply(sims[[7]], function(x){ lines(x$f ~ x$GEN, col = asex_blue, lwd = 1.4) })


title("modifier initial freq 0.1", outer=TRUE)

dev.off()