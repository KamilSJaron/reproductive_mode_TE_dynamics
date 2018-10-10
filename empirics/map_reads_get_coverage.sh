#!/bin/bash

#get variables to use in bash from two columns of a file per row

#merged masked genome and TEs bwa indexed file needed in working directory

#folder needed:
# processed_reads
# map
# out_bam
# ppileup_out

#add modules
module add UHTS/Aligner/bwa/0.7.13;
module add UHTS/Analysis/samtools/1.3;
module add Development/java/1.8.0_121;

clear
while read line; do
  read -a twoSp <<< $line
  printf "\n****processing ${twoSp[0]}*****\n"
  SP1=${twoSp[0]}

#mapping (-t is number of CPUs)
printf "**mapping $SP1**\n\n"
bwa bwasw -t 24 w303_ref_TE_sac_merged.fa ./processed_reads_trim/$SP1''_1_paired.fq > ./map/$SP1''_1.sam;
bwa bwasw -t 24 w303_ref_TE_sac_merged.fa ./processed_reads_trim/$SP1''_2_paired.fq > ./map/$SP1''_2.sam;

#merge mapped fw and rv reads with PopoolationTE2 java program
java -jar ../software/popte2-v1.10.04.jar se2pe --fastq1 ./processed_reads_trim/$SP1''_1_paired.fq \
--fastq2 ./processed_reads_trim/$SP1''_2_paired.fq \
--bam1 ./map/$SP1''_1.sam --bam2 ./map/$SP1''_2.sam --sort --output ./out_bam/$SP1''.sort.bam >> se2pe.log 2>&1;


#remove unnecessary files
  rm ./map/$SP1''_1.sam;
  rm ./map/$SP1''_2.sam;
  
  #rm ./out_bam/$SP1''.sort.bam;


done < ./samplenames


#get read stats from PopoolationTE:
cd ./out_bam
for f in *.bam; do java -jar ../../software/popte2-v1.10.04.jar stat-reads --bam $f --hier ../te-hierarchy.txt --output $f.read_stat.txt; done


module rm UHTS/Aligner/bwa/0.7.13;
module rm UHTS/Analysis/samtools/1.3;
module rm Development/java/1.8.0_121;



printf "****DONE****\n"


