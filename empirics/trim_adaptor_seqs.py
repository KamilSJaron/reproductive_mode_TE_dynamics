from sys import argv

# Check each read in a fastq file for the presence of a known adaptor sequence and remove the
# right end of the read and Q-score line from the start of the adaptor seq.
# Outputs a fastq file with the trimmed reads and a text file with the discarded sequence.

# Arguments are:
# prefix: the name of the input file without the .fastq extension
# bases_to_match: the number of bases in the adaptor sequence to look for in the fastq file
script, prefix, bases_to_match = argv

in_fn = prefix + '.fastq'
out_fn = prefix + '.trimmed.fastq'
discard_fn = prefix + '.discard.txt'

adaptor_seq = 'CTGTCTCTTATACACATCT'
search_seq = adaptor_seq[:int(bases_to_match)]

infile = open(in_fn, 'r')
outfile = open(out_fn, 'w')
discardfile = open(discard_fn, 'w')

line_n = 0
for line in infile:
    # Sequence line
    if line_n % 4 == 1:       
        # Look for the adaptor sequence
        search_loc = line.find(search_seq)
        if search_loc > 0:
            # Trim the adaptor seq and everything following it and write this to the discard file
            newline = line[:search_loc] + '\n'
            discardfile.write(line[search_loc:])
            flag = True
        else:
            newline = line
            flag = False

    # Quality score line    
    elif line_n % 4 == 3:
        # If the previous sequence line had the adaptor seq, trim the quality scores
        if flag:
            newline = line[:search_loc] + '\n'
        else:
            newline = line

    # Other lines
    else:
        newline = line

    outfile.write(newline)
    line_n += 1

infile.close()
outfile.close()
discardfile.close()

