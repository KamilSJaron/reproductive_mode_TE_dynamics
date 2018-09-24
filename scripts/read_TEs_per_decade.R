read_TEs_per_decade <- function(filename){
    df <- read.table(filename, header = F, sep = '\t', col.names = c('GEN', 'n_pre', 'n', 'f'))
    lapply(1:10, function(x){df[c(1:99) + (99 * (x - 1)),]} )
}