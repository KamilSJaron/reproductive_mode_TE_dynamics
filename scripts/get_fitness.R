get_fitness <- function(n, a, b){
    exp(- (a * n) - ((b * n^2) / 2))
}