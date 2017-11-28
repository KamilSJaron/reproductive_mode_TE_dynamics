get_fitness <- function(n, a, b){
    exp(- (a * n) - ((b * n^2) / 2))
}


# number of transposons
n = 1:200
# a parameters to explore
as = c(0.00001, 0.0001, 0.001, 0.01, 0.1)
# b parameters to explore
bs = c(0.00001, 0.0001, 0.001, 0.01, 0.1)

fitnesses = list(list())

png('figures/fitness_parametrization.png')

plot(NULL,
    xlim = c(1, max(n)), ylim = c(0,1),
    xlab = 'number of TEs', ylab = 'fitness',
    main = 'parametrization of fitness function')

for (i in 1:length(as)) {
    for (y in 1:length(bs)) {
        a = as[i]
        b = bs[y]
        fitness_vector <- get_fitness(n, a, b)
        fitnesses[[y + ((i - 1) * length(bs))]] <- fitness_vector
        lines(fitness_vector, lty = i, col = y)
    }
}

legend(175, 1.05, legend = bs, title = 'b', pch = 20, col = 1:length(bs), bty = 'n')
legend(140, 1.05, legend = as, title = 'a', pch = 20, lty = 1:length(as), bty = 'n')

dev.off()