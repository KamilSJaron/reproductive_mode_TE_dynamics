source('scripts/get_fitness.R')

# I know that yeast strain had ~50 TEs
# and I know that fitness decrease with this number is ~2%
# I can find as and bs so 50 -> 51 will make ~2% fitness difference
# i.e. I try to minimize function g(a,b) = f(50,a,b) - 0.98 * f(51,a,b)
# where f is fitness funcrtion (implemented in get_fitness)

# a parameters to explore
as = seq(0, 0.001, by = 0.00001)

# b parameters to explore
bs = seq(0, 0.001, by = 0.00001)

selections <- merge(as, bs)
colnames(selections) <- c('as', 'bs')

selections$fitness_50 <- apply(selections, 1, function(x){ get_fitness(50, x[1], x[2]) } )
selections$fitness_51 <- apply(selections, 1, function(x){ get_fitness(51, x[1], x[2]) } )
selections$dist_to_ideal_fitness_decrease <- abs(0.98 * selections$fitness_50 - selections$fitness_51)

winner <- selections[which.min(selections$dist_to_ideal_fitness_decrease),]
winner

# plot(selections$dist_to_ideal_fitness_decrease)
# image(matrix(selections$dist_to_ideal_fitness_decrease, nrow = length(as)))
# 
# plot(selections$fitness_50, pch = 20)
# points(0.98 * selections$fitness_50, pch = 20, col = 2)
# points(selections$fitness_51, pch = 20, col = 3)

png('figures/default_fitness_function.png')

plot(get_fitness(1:300, winner$as, winner$bs), pch = 20,
     xlab = 'number of TEs', ylab = 'relative fitness',
     main = 'simulation model')

lines(c(41,41), c(-0.2,1.2))
lines(c(50,50), c(-0.2,1.2))

legend('topright', bty = 'n', legend = c('simulation space', paste('a = ', winner$as), paste('b = ', winner$bs)), lty = c(1, NA, NA))

dev.off()