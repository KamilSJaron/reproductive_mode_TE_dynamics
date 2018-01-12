read_simulation <- function(filename){
	file <- readLines(filename)

	sim <- list()
	i = 1

	for( line_index in which(grepl("chrom", file)) ){
		from = line_index + 2
		to = from
		while(file[to] != ""){
			to = to + 1
			if(is.na(file[to])){
				break
			}
		}
		one_sim_lines <- file[from:(to-1)]
		header <- unlist(strsplit(one_sim_lines[1], split = '\t'))
		temp_df <- t(sapply(one_sim_lines[-1], function(x){as.numeric(unlist(strsplit(x, split = '\t')))}))
		rownames(temp_df) <- c(1:nrow(temp_df))
		colnames(temp_df) <- header
		sim[[i]] <- as.data.frame(temp_df)
		i = i + 1;
	}

	return(sim)
}