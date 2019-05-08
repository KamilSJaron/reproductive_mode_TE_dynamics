#!/usr/bin/env Rscript

asex_blue <- "#92C5DECD"
sex_red <- "#D6604DED"
# attempts to coverage filter
# get_KAT_hist <- function(filename, cov_treshold = 2){
#      hist <- read.table(filename)
#      sum(hist$V2[hist$V1 > cov_treshold])
# }
# hist1 <- 'data/kmer_overlaps/6D_6C_90/overlap.1.hist'
# hist2 <- 'data/kmer_overlaps/6D_6C_90/overlap.2.hist'

#####
# misc
#####

extract <- function(file, line, pos){
     as.numeric(file[[line]][pos])
}

#####
# function to extract a metric from overlap KAT files
#####

getDistance <- function(s1, s2, gen, metric = 'fraction'){
     if ( grepl("[A-Z]", s2) ){
          # if I ask for sample, sample, gen
          .stats_filename <- paste0('data/kmer_overlaps/', s1, '_', s2, '_', gen, '/overlap.stats')
     } else {
          # if I ask for sample, gen, gen
          .stats_filename <- paste0('data/kmer_overlaps/', s1, '/', s2, '_', gen, '.stats')
     }

     if ( !file.exists(.stats_filename) ){
          return( NA )
     }
     raw_file <- strsplit(readLines(.stats_filename), ' ')
     if ( metric == 'fraction' ){
          shared_kmers <- extract(raw_file, 24, 6)
          sample1_total <-  extract(raw_file, 10, 5)
          return( shared_kmers / sample1_total )
     }
     # extract(27,5) - Manhattan distance
     if ( metric == 'Manhattan' ){
          return( extract(raw_file, 27,5) )
     }
     # extract(28,5) - Euclidean distance
     if ( metric == 'Euclidean' ){
          return( extract(raw_file, 28,5) )
     }
     # extract(29,5) - Cosine distance
     if ( metric == 'Cosine' ){
          return( extract(raw_file, 29,5) )
     }
     # extract(30,5) - Canberra distance
     if ( metric == 'Canberra' ){
          return( extract(raw_file, 30,5) )
     }
     # extract(31,5) - Jaccard distance
     if ( metric == 'Jaccard' ){
          return( extract(raw_file, 31,5) )
     }
     return( NA )
}

generation_means <- function(s1, s2, .generations, method = 'fraction'){
     mean(sapply(.generations, function(gen){ getDistance(s1, s2, gen, method) }), na.rm = T)
}

######
# script to do all
######

samples <- read.table('data/samples.txt', stringsAsFactors = F)$V1
generations <- read.table('data/generations.txt', stringsAsFactors = F)$V1

variables <- c('metric', 'early_sex_mean',  'early_sex_var',
                         'early_asex_mean', 'early_asex_var',
                         'late_sex_mean',   'late_sex_var',
                         'late_asex_mean',  'late_asex_var')
asm_template <- matrix(ncol = length(variables))
colnames(asm_template) <- variables
result_table <- as.data.frame(asm_template)[FALSE, ]
for ( metric in c('fraction', 'Manhattan', 'Euclidean', 'Cosine', 'Canberra', 'Jaccard')){
     early_pairwise_sample_overalaps = matrix(0, ncol = length(samples), nrow = length(samples))
     late_pairwise_sample_overalaps = matrix(0, ncol = length(samples), nrow = length(samples))
     for ( s1 in samples ){
          for ( s2 in samples ){
               if ( s1 != s2 ){
                    x = which(s1 == samples)
                    y = which(s2 == samples)
                    early_pairwise_sample_overalaps[x, y] <- generation_means(s1, s2, generations[1:4], metric)
                    late_pairwise_sample_overalaps[x, y] <-  generation_means(s1, s2, generations[8:12], metric)
               }
          }
     }
     early_sex <- as.vector(early_pairwise_sample_overalaps[1:4,1:4])
     early_asex <- as.vector(early_pairwise_sample_overalaps[5:8,5:8])
     late_sex <- as.vector(late_pairwise_sample_overalaps[1:4,1:4])
     late_asex <- as.vector(late_pairwise_sample_overalaps[5:8,5:8])

     result_table <- rbind(result_table,
                           data.frame(metric = metric,
                                      early_sex_mean = mean(early_sex),
                                      early_sex_var = var(early_sex),
                                      early_asex_mean = mean(early_asex),
                                      early_asex_var = var(early_asex),
                                      late_sex_mean = mean(late_sex),
                                      late_sex_var = var(late_sex),
                                      late_asex_mean = mean(late_asex),
                                      late_asex_var = var(late_asex)))

     plotnames <- paste0('figures/kmer_distance_', metric, c('_early', '_late'), '.png')
     png(plotnames[1])
          pic_title <- paste('early generations using', metric)
          par(mar = c(2.1, 4.1, 5.1, 2.1))
          image(early_pairwise_sample_overalaps, axes = FALSE, main = pic_title)
          axis(3, at = seq(0, 1, len = 8), labels = samples, tick = F)
          axis(2, at = seq(0, 1, len = 8), labels = samples, tick = F)
     dev.off()

     png(plotnames[2])
          pic_title <- paste('late generations using', metric)
          par(mar = c(2.1, 4.1, 5.1, 2.1))
          image(late_pairwise_sample_overalaps, axes = FALSE, main = pic_title)
          axis(3, at = seq(0, 1, len = 8), labels = samples, tick = F)
          axis(2, at = seq(0, 1, len = 8), labels = samples, tick = F)
     dev.off()
}

for ( metric in c('fraction', 'Manhattan', 'Euclidean', 'Cosine', 'Canberra', 'Jaccard')){
     png(paste0('figures/summary_means_', metric, '.png'))
     bars = as.vector(unlist(result_table[result_table$metric == metric, seq(2, 9, by = 2)]))
     names(bars) <- paste(rep(c('early', 'late'), each = 2))
     barplot(bars, col = c(sex_red, asex_blue), main = paste('mean', metric))
     dev.off()

     png(paste0('figures/summary_vars_', metric, '.png'))
     bars = as.vector(unlist(result_table[result_table$metric == metric, seq(3, 9, by = 2)]))
     names(bars) <- paste(rep(c('early', 'late'), each = 2))
     barplot(bars, col = c(sex_red, asex_blue), main = paste('var', metric))
     dev.off()
}

######################
##### TIME SERIES ####
######################

asm_template <- matrix(ncol = length(variables))
colnames(asm_template) <- variables
result_table <- as.data.frame(asm_template)[FALSE, ]
for ( metric in c('fraction', 'Manhattan', 'Euclidean')){
     for ( s in samples ){
          generation_overalaps = matrix(0, ncol = length(generations), nrow = length(generations))
          for ( g1 in generations ){
               for ( g2 in generations ){
                    if ( g1 != g2 ){
                         x <- which(g1 == generations)
                         y <- 12 - which(g2 == generations)
                         generation_overalaps[x, y] <- getDistance(s, g1, g2, metric)
                    }
               }
          }
          plotname <- paste0('figures/kmer_distance_time_', s, '_', metric, '.png')
          png(plotname)
               pic_title <- paste(s, ' using', metric)
               par(mar = c(2.1, 4.1, 5.1, 2.1))
               image(generation_overalaps, axes = FALSE, main = pic_title)
               axis(3, at = seq(0, 1, len = 11), labels = generations, tick = F)
               axis(2, at = seq(0, 1, len = 11), labels = rev(generations), tick = F)
          dev.off()
     }
}