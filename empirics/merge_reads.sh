#!/bin/bash
#merge overlapping read pairs for yeast

module add UHTS/Assembler/pear/0.9.6;

clear
while read line; do
  read -a twoSp <<< $line
  printf "\n****processing ${twoSp[0]}*****\n"
  SP1=${twoSp[0]}
 
#decompress reads
  #printf "**decompressing $SP1**\n\n"
  #gunzip /scratch/beegfs/monthly/jbast/yeast/processed_reads_trim/$SP1''_1_paired.fq.gz
  #gunzip /scratch/beegfs/monthly/jbast/yeast/processed_reads_trim/$SP1''_2_paired.fq.gz
 
#run pipe
  printf "**running $SP1**\n\n"
  pear \
  -r /scratch/beegfs/monthly/jbast/yeast/processed_reads_trim/$SP1''_1_paired.fq \
  -f /scratch/beegfs/monthly/jbast/yeast/processed_reads_trim/$SP1''_2_paired.fq \
  -o /scratch/beegfs/monthly/jbast/yeast/merge_reads/$SP1''_merged
 
done < samplenames

module rm UHTS/Assembler/pear/0.9.6;

printf "****DONE****\n"
