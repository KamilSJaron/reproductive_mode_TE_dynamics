transposition_rates = c("0.0001", "0.00001", '0.000001') # * 100, *10
excision_rates = c("0.00005", "0.000001", "0.0000005") # *2, /2
selection_a <- c("0.00051", "0.0001")
selection_b <- c("0.00039", "0")

sim_tab <- merge(merge(merge(data.frame(transposition_rates = transposition_rates),
                             data.frame(excision_rates = excision_rates)),
                             data.frame(selection_a = selection_a)),
                             data.frame(selection_b = selection_b))

# Stupid me, I duplicated one sim
sim_tab <- sim_tab[c(12, 1:11, 12:36),]
common_dir <- 'sims/01_Dolgin_and_Charlesworth_model'

dir.create(common_dir)
for(i in 1:nrow(sim_tab)){
    print_index <- ifelse(i < 10, paste0('0',i), as.character(i))
    dirname <- paste0(common_dir, '/1_', print_index, "_parameter_expl")
    dir.create(dirname)
    input_name <- paste0(dirname, '/input.txt')
    input_tab <- c("100000", "0", as.character(unlist(sim_tab[i, c(1,1,2,2,3,4)])), "50")
    writeLines(input_tab, input_name)
}
