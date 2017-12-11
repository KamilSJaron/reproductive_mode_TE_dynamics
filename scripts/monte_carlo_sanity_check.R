
fitnesses <- c(0.5,0.2,0.7,0.9,1)
replicates <- 10000

get_ind <- function(x){
    while ( TRUE ) {
        ind = ceiling(runif(1, min = 0, max = length(fitnesses)))
        fitness = fitnesses[ind]
        if (runif(1) < fitness){
            return(ind)
        }
    }
}

selected_ind <- unlist(lapply(1:replicates, get_ind))

data.frame(relative_fitness = fitnesses / sum(fitnesses),
           mc_prob = (table(selected_ind) / replicates))