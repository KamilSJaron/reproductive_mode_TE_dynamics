read_input <- function(filename){
    file <- readLines(filename)
    file <- file[file != ""] # erase empty lines

    values <- as.numeric(file)
    dirname <- strsplit(filename, '/')[[1]][2]

# to have backward compatibility for simulaiton wo general modifier
    if(length(file) == 7){
        data.frame(dir = dirname,
                   popSize = values[1],
                   transp_mitosis = values[2],
                   transp_meiosis = values[3],
                   exision = values[4],
                   selection_a = values[5],
                   selection_b = values[6],
                   initial_TEs = values[7])
    } else {
        data.frame(dir = dirname,
                   popSize = values[1],
                   transp_mitosis = values[2],
                   transp_meiosis = values[3],
                   exision_base = values[4],
                   exision_modified = values[5],
                   selection_a = values[6],
                   selection_b = values[7],
                   initial_TEs = values[8],
                   initial_freq_of_modifier = values[9])
    }
}