png('figures/ranges_of_the_fitness_function.png')

source('scripts/get_fitness.R')

label <- merge(data.frame(a = c("0.1      ", "0.01    ", "0.001  ", "0.0001")),
               data.frame(b = c("0.001 ", "0.0001")))
label <- paste(label$a, label$b)

parameters <- merge(data.frame(a = c(0.1000, 0.0100, 0.0010, 0.0001)),
                    data.frame(b = c(0.001, 0.0001)))
parameters$col <- rep(c('black', 'red'), each = 4)
parameters$lty <- rep(1:4, 2)

plot(NULL, xlim = c(0,100), ylim = c(0, 1), main = 'Fitness function',
     xlab = "Number of TEs (n)", ylab = "Fitness (w)")

for(i in 1:nrow(parameters)){
     lines(get_fitness(a = parameters$a[i], b = parameters$b[i], n = 1:100),
           col = parameters$col[i], lty = parameters$lty[i])
}

legend('topright', bty = 'n', title = "a          b", label,
       col = parameters$col, lty = parameters$lty)

dev.off()