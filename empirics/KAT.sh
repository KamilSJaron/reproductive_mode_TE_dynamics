#!/bin/bash

export PATH=$PATH:/scratch/beegfs/monthly/ptranvan/Software/KAT/2.4.1/kat_tmp/bin
export PYTHONPATH=$PYTHONPATH:/software/lib64/python3.5/site-packages

# `data/samples.txt` contains the 8 samples

mkdir -p data/mapped_reads
mkdir -p data/kmer_overlaps

##
## wihin generation
##

for sample1 in $(cat data/samples.txt); do
    for sample2 in $(cat data/samples.txt); do
        for gen in $(cat data/generations.txt); do
            sam_file_name_s1="data/mapping/"$sample1"-"$gen".btsub.mapped.sam";
            sam_file_name_s2="data/mapping/"$sample2"-"$gen".btsub.mapped.sam";
            if [[ $sample1 != $sample2 ]] && [[ -e $sam_file_name_s1 ]] && [[ -e $sam_file_name_s2 ]]; then
                # echo comparing $sample1 and $sample2 at g $gen
                fasta_s1="data/mapped_reads/"$sample1"-"$gen".btsub.mapped.fasta"
                fasta_s2="data/mapped_reads/"$sample2"-"$gen".btsub.mapped.fasta"
                if [[ ! -e $fasta_s1 ]]; then
                    awk '{ print ">" NR  "\n" $10 }' $sam_file_name_s1 > $fasta_s1
                fi

                if [[ ! -e $fasta_s2 ]]; then
                    awk '{ print ">" NR  "\n" $10 }' $sam_file_name_s2 > $fasta_s2
                fi

                out_pattern="data/kmer_overlaps/"$sample1"_"$sample2"_"$gen"/overlap"
                if [[ ! -e "$out_pattern".stats ]]; then
                    mkdir -p $(dirname $out_pattern)
                    kat comp -o $out_pattern -t 16  -m 21 -v --output_hists $fasta_s1 $fasta_s2 2> /dev/null
                fi
            fi
        done
    done
done

#
# within sample
#


for sample in $(cat data/samples.txt); do
    for gen1 in $(cat data/generations.txt); do
        for gen2 in $(cat data/generations.txt); do
            sam_file_name_g1="data/mapping/"$sample"-"$gen1".btsub.mapped.sam";
            sam_file_name_g2="data/mapping/"$sample"-"$gen2".btsub.mapped.sam";
            if [[ $gen1 != $gen2 ]] && [[ -e $sam_file_name_g1 ]] && [[ -e $sam_file_name_g2 ]]; then
                echo comparing $sample at $gen1 and $gen2
                fasta_g1="data/mapped_reads/"$sample"-"$gen1".btsub.mapped.fasta"
                fasta_g2="data/mapped_reads/"$sample"-"$gen2".btsub.mapped.fasta"
                if [[ ! -e $fasta_g1 ]]; then
                    awk '{ print ">" NR  "\n" $10 }' $sam_file_name_g1 > $fasta_g1
                fi

                if [[ ! -e $fasta_g2 ]]; then
                    awk '{ print ">" NR  "\n" $10 }' $sam_file_name_g2 > $fasta_g2
                fi

                out_pattern="data/kmer_overlaps/"$sample"/"$gen1"_"$gen2""
                if [[ ! -e "$out_pattern".stats ]]; then
                    mkdir -p $(dirname $out_pattern)
                    kat comp -o $out_pattern -t 16 -m 21 -v --output_hists $fasta_g1 $fasta_g2 2> /dev/null
                fi
            fi
        done
    done
done