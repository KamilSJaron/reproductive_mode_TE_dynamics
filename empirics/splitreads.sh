#!/bin/bash
#split merged overlapping read pairs in two for yeast 
#and delete for forward SIZE (eg. 20) bases and reverse SIZE bases to produce artificial paired reads

module add UHTS/Analysis/trimmomatic/0.33;
module add UHTS/Analysis/fastx_toolkit/0.0.13.2;

clear
while read line; do
  read -a twoSp <<< $line
  printf "\n****processing ${twoSp[0]}*****\n"
  SP1=${twoSp[0]}

SIZE=20

cd merge_reads

#split merged reads exactly in half
  cat $SP1''_merged.assembled.fastq | awk 'NR%2==1 { print $0 } ; NR%2==0 { print substr($0,0,length($0)/2) }' >  $SP1''_merged.split1.temp
  cat $SP1''_merged.assembled.fastq | awk 'NR%2==1 { print $0 } ; NR%2==0 { print substr($0,length($0)/2+1) }'>  $SP1''_merged.split2.temp

#cut number of bases from end of forward and beginning of reverse reads
  fastx_trimmer -i $SP1''_merged.split1.temp -o $SP1''_merged.splittrim''$SIZE''.f.fastq -t $SIZE -Q33
  fastx_trimmer -i $SP1''_merged.split2.temp -o $SP1''_merged.splittrim''$SIZE''.r.fastq -f $SIZE -Q33

#remove temporary files
  rm $SP1''_merged.split1.temp
  rm $SP1''_merged.split2.temp

#merge with unassembled reads

  cat $SP1''_merged.splittrim''$SIZE''.f.fastq $SP1''_merged.unassembled.forward.fastq > $SP1''_combined''$SIZE''f.fastq 
  cat $SP1''_merged.splittrim''$SIZE''.r.fastq $SP1''_merged.unassembled.reverse.fastq > $SP1''_combined''$SIZE''r.fastq


cd ..

done < samplenames

module rm UHTS/Analysis/trimmomatic/0.33;
module rm UHTS/Analysis/fastx_toolkit/0.0.13.2;

printf "****DONE****\n"
