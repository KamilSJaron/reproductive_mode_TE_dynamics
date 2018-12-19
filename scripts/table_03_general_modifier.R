# script for work in progress, this is just for the case that some of it would be usefull latter on

library(RColorBrewer)
# source('scripts/read_simulation.R')
source('scripts/read_input.R')
source('scripts/read_TEs_per_decade.R')

common_sim_dir <- 'sims/03_general_modifier_of_exision_rates'
tab_file <- c('sims/03_general_modifier_of_exision_rates_table.tsv')
sims_dirs <- dir(common_sim_dir)
sims_files <- paste(common_sim_dir, sims_dirs, rep(c('sex_TEs_per_decade_2.tsv','asex_TEs_per_decade_2.tsv'), each = length(sims_dirs)), sep = '/')
input_files <- paste(common_sim_dir, sims_dirs, 'input.txt', sep = '/')

asex_blue <- "#92C5DECD"
sex_red <- "#D6604DED"

pal <- c(sex_red, asex_blue)

sim_inputs <- lapply(input_files, read_input)

get_values <- function(variable){
     sapply(sim_inputs, function(x){ x[,variable] })
}

input_table <- data.frame(dir = get_values('dir'),
                          init_f = get_values('initial_freq_of_modifier'),
                          selection_a = get_values('selection_a'),
                          selection_b = get_values('selection_b'))

sims <- lapply(sims_files, read_TEs_per_decade)

one_plot <- function(i_sex, i_asex, title){
     plot(c(0, 1000), c(50,50),
          xlim = c(1, 990),
          ylim = c(1, 60), type = 'l',
          xlab = 'Generation', ylab = 'Mean number of TEs',
          bty = 'n', cex.axis = 1.4, cex.lab = 1.4, cex.main = 1.6)

     sapply(sims[[i_sex]], function(x){ lines(x$n ~ x$GEN, col = sex_red, lwd = 1.4) })
     sapply(sims[[i_asex]], function(x){ lines(x$n ~ x$GEN, col = asex_blue, lwd = 1.4) })

     legend('bottomleft', c('sex', 'asex'),
            bty = 'n', pch = 20,
            cex = 1.4, col = pal)

     plot(NULL,
          xlim = c(1, 990),
          ylim = c(0, 1),
          xlab = 'Generation', ylab = 'frequency of the modifier in population',
          bty = 'n', cex.axis = 1.4, cex.lab = 1.4, cex.main = 1.6)

     sapply(sims[[i_sex]], function(x){ lines(x$f ~ x$GEN, col = sex_red, lwd = 1.4) })
     sapply(sims[[i_asex]], function(x){ lines(x$f ~ x$GEN, col = asex_blue, lwd = 1.4) })

     title(title, outer=TRUE)
}

number_of_sims <- nrow(input_table)
for (input_index in 1:number_of_sims){
     input <- input_table[input_index,]
     title <- paste0('init_f = ', input[2], ', a = ', input[3], ', b = ', input[4])
     file <- paste0('figures/03_init_f', input[2], '_a', input[3], '_b', input[4], '.pdf')

     pdf(file, width = 18, height = 9)
          par(mfrow=c(1,2), oma=c(0,0,1.5,0), mar = c(5, 4, 1, 2) + 0.1)
          one_plot(input_index, input_index + number_of_sims, title)
     dev.off()
}

sim_table <- unique(input_table[,c(2,3,4)])
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

table_to_save <- sim_table[
  with(sim_table, order(init_f, selection_b, selection_a)),
]

write.table(table_to_save, tab_file, quote = F, sep = '\t', row.names = F)
