# based on a 'lab script' written by Kamil S. Jaron
# proudly released to public domain
# taken from my gist: https://gist.github.com/KamilSJaron/358c997698b67486be47d4e8eef2921d

# args :
# response - response variable;
# var1, var2, var3 - threee variables in a model (`response ~ v1 + v2 + v3 + v2:v3` which is same as `response ~ v1 + v2*v3`);
# nprep - number of permutation replicates;
# plot_name - is speficied a pdf with histograms will be generated
# number of cores used for computation - default is number of processors - 1; SPECIFY THIS PARAMETER ON BIG SERVERS;

require(parallel)

papanova <- function(response, var1, var2, var3, nprep = 10, plot_name = NA, no_cores = NA){
    if(is.na(no_cores)){
        no_cores <- detectCores() - 1
    }
    cl <- makeCluster(no_cores, type="FORK")

    # compute reference anova ( x == 1)
    # amd compute nprep - 1 replicates ( x != 1) of anova with permutation of response variable
    # premutations is a matrix (rows are variables, columns are replicates)
    premutations <- parSapply(   cl, 1:nprep,
                                 function(x){
                                    if(x == 1){
                                        anova(lm(response ~ var1 + var2 * var3))$F
                                    } else {
                                        anova(lm(sample(response) ~ var1 + var2 * var3))$F
                                    }
                                }
                             )
    stopCluster(cl)

    # calculate p-values of individual variables
    pval <- NULL
    for (j in 1:4) {
        pval[j] <- sum(premutations[j,] >= premutations[j,1]) / nprep
    }

    # in filename is specified, plot a picture
    if(!is.na(plot_name)){
        pdf(plot_name)
            par(mfrow=c(4,1))
            hist(premutations[1,], xlab="F", main = 'Factor 1')
            abline(v = premutations[1,1], lwd=2)
            hist(premutations[2,], xlab="F", main = 'Factor 2')
            abline(v = premutations[2,1], lwd=2)
            hist(premutations[3,], xlab="F", main = 'Factor 3')
            abline(v = premutations[3,1], lwd=2)
            hist(premutations[4,], xlab="F", main = 'Interaction')
            abline(v = premutations[4,1], lwd=2)
        dev.off()
    }
    return(pval)
}