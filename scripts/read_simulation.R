read_simulation <- function(filename){
	file <- readLines(filename)

	sim <- list()

	for(i in 1:10){
	    one_sim_lines <- file[(11 + (i - 1) * 23):(23 + (i - 1) * 23)]
	    header <- unlist(strsplit(one_sim_lines[1], split = '\t'))
	    temp_df <- t(sapply(one_sim_lines[-1], function(x){as.numeric(unlist(strsplit(x, split = '\t')))}))
	    rownames(temp_df) <- c(1:12)
	    colnames(temp_df) <- header
	    sim[[i]] <- as.data.frame(temp_df)
	}

	return(sim)
}