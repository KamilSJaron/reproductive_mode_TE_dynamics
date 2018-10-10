#!/bin/bash

printf "\nprocessing...\n\n"
for f in *.read_stat.txt; do

 #reformat file
 cat $f | sed 's/chr\t/chr_/g' | sed 's/^te\t/te_/g' > temp;
 #get total number of mapped reads as variable
 totalreads=$(awk 'FNR==3 {print $2}' temp);
 
 #add column with fraction
 awk -v var="$totalreads" '{print $1"\t"$2"\t"$2/var}' temp > $f.2;
 
 #remove temp file 
 rm temp;

done

#get fraction of reads that mapped to TEs out of total mapped
for f in *txt.2; do awk 'BEGIN{OFS="\t"}{print $0, FILENAME}' $f; done | grep 'reads_mapped_to_te_with_mq' | cut -f 3,4 | \
sed 's/.sort.bam.read_stat.txt.2//g' | sed 's/\(.*\)-/\1 /' > read_stats.txt
