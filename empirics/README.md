### Gather reads

Reads downloaded from the SRA (BioProject identifier [PRJNA308843](https://www.ncbi.nlm.nih.gov/bioproject/PRJNA308843/))

- rename sra according to samples:

```
for file in *.sra; do read line;  mv -v "${file}" "${line}";  done < ../Names.txt
```

- convert sra to fastq:

```
for f in *.sra; do ~/Software/sratoolkit.2.5.7-ubuntu64/bin/fastq-dump.2.5.7 --gzip --split-3 $f; done
```

### Raw read quality filtering and processing:

- removing adapter sequences (script by Dan Rice):

```
trim_adaptor_seqs.py
```

- read quality trimming:

```
jobscript_trimmomatic.sh
```

- merge reads of mated sexual strains and and renaming:

```
merge_sexpair_reads.sh
```

- creation of 'artifical' non-overlapping paired-end reads to allow usage for insertion pipeline

```
merge_reads.sh
```

```
splitreads.sh
```

### TE abundance estimates

- identification of TE content and positions in the ancestral 'starting' strain W303 genome using RepeatMasker and a curated yeast TE library:

```
 ~/Software/RepeatMasker406/RepeatMasker -nolow -lib sac_cer_TE_seqs_bergman.fasta -gccalc -s -cutoff 200 -no_is -nolow -norna -gff -u -engine rmblast -pa 8 w303_ref.fasta
```

- mapping reads to the masked W303 genome with appended TE library with bwa-mem and get coverage for each element (following PopoolationTE2 guidelines):

```
map_reads_get_coverage.sh
```

- to calculate fraction of reads mapping

```
read_stat_processing.sh
```

### TE insertion estimates

- run the McClintock pipeline to get TE insertion estimates (run on Vital-it cluster)

```
modules_mcclintock.sh
```

```
run_McClintock_pipe.sh
```

- collect the "nonredundant" results from the McClintock output folder:

```
find ./w303_ref/ -name '*_nonredundant.bed' -exec cp {} ./collected_results/ \;
```

```
for f in *.bed; do awk 'BEGIN{OFS="\t"}{print $0, FILENAME}' $f | sed '1d'; done | sed 's/_nonredundant.bed//g' | sed 's/_combined20f.fastq_/\t/g' | sed 's/\(.*\)-/\1 /' | awk '{print$0"\t"$4}' | sed 's/\(.*\)_/\1\t/' | cut -f 1-10 | sed 's/\(.*\)_/\1\t/' | awk '{print$0"\t"$5}' | cut -f 1-3,6-13 | sed 's/_sr/\tsr/g' | sed 's/_rp/\trp/g' | sed 's/_nonab/\tnonab/g' | sed 's/\(.*\)_/\1\t/' | cut -f 1-3,5-6,8-11 > collected_nonredundant_results.tbl
```

- remove TEMP "nonab" inserts

```
cat collected_nonredundant_results.tbl2 | grep -v 'nonab' > collected_nonredundant_results_no_nonab.tbl2
```

- collect all evidence for insertions of all different programs and merge overlapping evidence within 200 bp boundaries (script provided by Dan Jeffries)

```
TE_insertion_merger.py
```

- add insertion position to the files

```
cat collected_nonredundant_results_no_nonab_merged.tbl2 | sed 's/(//g' | sed 's/)//g' | cut -f 12 | sed 's/-/\t/g' | cut -d ',' -f 1 > startend
```

```
paste collected_nonredundant_results_no_nonab_merged.tbl2 startend > collected_nonredundant_results_no_nonab_merged.tbl3
```

### Read coverage per sample

- get coverage for each sample from McClintock generated bwa mapping:
```
find ./w303_ref/ -name '*.sorted.bam' -exec sh -c 'bedtools genomecov -d -ibam $0 -g w303_ref/reference/w303_ref.fasta > $0.cov' {} \; find ./w303_ref/ -name '*.cov' -exec cp {} ./coverage/ \;
```
- calculate median coverage from these files

```
for f in *.cov; do sort -n -k3 $f | awk -v var="$f" ' { a[i++]=$3; } END { print var,a[int(i/2)];}' | sed 's/.sort.bam.cov//g' | sed 's/\(.*\)-/\1 /'; done >> cov.median
```

### Analyses
- statistics, evaliation and length filtering of TEs all in the R script
```
yeast_TEloads.R
```

