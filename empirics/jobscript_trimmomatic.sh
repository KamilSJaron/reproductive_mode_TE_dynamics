#!/bin/bash -x

#BSUB -L /bin/bash
#BSUB -o trim-%J-output.txt
#BSUB -e trim-%J-error.txt
##BSUB -N
#BSUB -u jbast@unil.ch
#BSUB -J trimmomatic
#BSUB -n 24
##BSUB -R "span[ptile=8]"
##BSUB -q long
#BSUB -R "rusage[mem=12000]"
#BSUB -M 12000000

module add UHTS/Analysis/trimmomatic/0.33;

while read line; do trimmomatic PE -phred33 ${line}_1.trimmed.fastq ${line}_2.trimmed.fastq ${line}_1_paired.fq.gz ${line}_1_unpaired.fq.gz ${line}_2_paired.fq.gz ${line}_2_unpaired.fq.gz \
LEADING:3 TRAILING:3 HEADCROP:10 SLIDINGWINDOW:4:15 MINLEN:36; done < sample_names;

module rm UHTS/Analysis/trimmomatic/0.33;
