source('scripts/read_input.R')
source('scripts/read_TEs_per_decade.R')

tab_file <- 'sims/01_Dolgin_and_Charlesworth_model_table.tsv'

common_sim_dir <- 'sims/01_Dolgin_and_Charlesworth_model'
sims_dirs <- dir(common_sim_dir)
sims_files <- paste(common_sim_dir, sims_dirs, rep(c('sex_TEs_per_decade_2.tsv','asex_TEs_per_decade_2.tsv'), each = length(sims_dirs)), sep = '/')
input_files <- paste(common_sim_dir, sims_dirs, 'input.txt', sep = '/')

sim_inputs <- lapply(input_files, read_input)

get_values <- function(variable){
     sapply(sim_inputs, function(x){ x[,variable] })
}

input_table <- data.frame(dir = get_values('dir'),
                          transposition_rate = get_values('transp_mitosis'),
                          exision_rate = get_values('exision_base'),
                          selection_a = get_values('selection_a'),
                          selection_b = get_values('selection_b'))

number_of_sims <- nrow(input_table)

sims <- lapply(sims_files, read_TEs_per_decade)
sim_table <- unique(input_table[,c(2:5)])
# sim_table$sex_slope <- NA
sim_table$sex_lost_TEs <- NA
sim_table$sex_lost_TEs_se <- NA
# sim_table$asex_slope <- NA
sim_table$asex_lost_TEs <- NA
sim_table$asex_lost_TEs_se <- NA

extract_col <- function(sublist, column){
  unlist(lapply(sublist, function(y){ as.vector(sapply(y, function(x){ x[,column] })) } ))
}

get_te_loss <- function(index){
     generations <- extract_col(sims[index], 1)
     lost_TEs <- 50 - extract_col(sims[index], 3)
     TE_mod <- lm(lost_TEs ~ generations)
     prediction <- predict(TE_mod, data.frame(generations = 990), se.fit = T)
     # TE_mod$coefficients[2] - slope
     return(c(prediction$fit, prediction$se.fit))
}


for (i in 1:nrow(sim_table)){

     indexes <- which(apply(input_table[,2:4], 1, function(x){ all(x == sim_table[i,1:3])}))

     sim_table[i, c('sex_lost_TEs', 'sex_lost_TEs_se')] <- get_te_loss(indexes)
     sim_table[i, c('asex_lost_TEs', 'asex_lost_TEs_se')] <- get_te_loss(indexes + number_of_sims)
}

sim_table[,c('sex_lost_TEs', 'asex_lost_TEs')] <- round(sim_table[,c('sex_lost_TEs', 'asex_lost_TEs')], 1)
sim_table[,c('sex_lost_TEs_se', 'asex_lost_TEs_se')] <- round(sim_table[,c('sex_lost_TEs_se', 'asex_lost_TEs_se')], 2)

write.table(sim_table, tab_file, quote = F, sep = '\t', row.names = F)