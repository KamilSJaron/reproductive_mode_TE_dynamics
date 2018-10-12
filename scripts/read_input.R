read_input <- function(filename){
    file <- readLines(filename)
    file <- file[file != ""] # erase empty lines

    values <- as.numeric(file)
    path <- strsplit(filename, '/')[[1]]
    dirname <- path[length(path) - 1]

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
                   initial_freq_of_modifier = values[2],
                   transp_mitosis = values[3],
                   transp_meiosis = values[4],
                   exision_base = values[5],
                   exision_modified = values[6],
                   selection_a = values[7],
                   selection_b = values[8],
                   initial_TEs = values[9])
    }
}