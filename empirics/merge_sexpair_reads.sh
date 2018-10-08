#!/bin/bash

#get variables to use in bash from two columns of a file per row

#merged masked genome and TEs bwa indexed file needed in working directory

#folder needed with reads:
# sra_files/processed_reads_qualtrim/


clear
while read line; do
  IFS='-' read -a twoSp <<< $line
  clear
  echo "Species pair processing ${twoSp[0]} - ${twoSp[1]}"
  SP1=${twoSp[0]}
  SP2=${twoSp[1]}


zcat ./sra_files/processed_reads_qualtrim/$SP1''-0_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-0_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-0_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-90_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-90_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-90_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-180_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-180_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-180_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-270_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-270_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-270_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-360_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-360_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-360_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-450_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-450_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-450_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-540_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-540_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-540_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-630_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-630_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-630_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-720_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-720_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-720_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-810_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-810_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-810_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-900_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-900_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-900_1_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-990_1_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-990_1_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-990_1_paired.fq.gz;

zcat ./sra_files/processed_reads_qualtrim/$SP1''-0_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-0_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-0_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-90_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-90_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-90_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-180_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-180_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-180_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-270_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-270_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-270_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-360_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-360_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-360_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-450_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-450_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-450_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-540_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-540_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-540_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-630_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-630_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-630_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-720_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-720_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-720_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-810_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-810_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-810_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-900_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-900_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-900_2_paired.fq.gz;
zcat ./sra_files/processed_reads_qualtrim/$SP1''-990_2_paired.fq.gz ./sra_files/processed_reads_qualtrim/$SP2''-990_2_paired.fq.gz | gzip > ./sra_files/processed_reads_qualtrim/$SP1''-''$SP2''-990_2_paired.fq.gz;


done < sexpair_names

printf "****DONE****\n"
