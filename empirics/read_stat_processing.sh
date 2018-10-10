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
