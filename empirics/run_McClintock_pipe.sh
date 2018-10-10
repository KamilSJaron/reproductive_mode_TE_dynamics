#!/bin/bash
#decompress fq.gz and run McClintock pipeline on the yeast data
 
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
 /scratch/beegfs/monthly/jbast/software/mcclintock/mcclintock/mcclintock.sh \
 -r /scratch/beegfs/monthly/jbast/yeast/w303_ref.fasta \
 -c /scratch/beegfs/monthly/jbast/yeast/sac_cer_TE_seqs_bergman_conv.fasta \
 -1 /scratch/beegfs/monthly/jbast/yeast/merge_reads/$SP1''_combined20f.fastq \
 -2 /scratch/beegfs/monthly/jbast/yeast/merge_reads/$SP1''_combined20r.fastq \
 -p 24 \
 -o /scratch/beegfs/monthly/jbast/yeast/PairComb20 &> output_PairComb20_$SP1''.log

#one could add the -i parameter for discarding intermediate files

 
done < samplenames_2

printf "****DONE****\n"
