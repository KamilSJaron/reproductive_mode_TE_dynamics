### Cline

import sys

if len(sys.argv) < 2:
    sys.exit("USAGE:\n\n\tTE_insertion_merger.py <full_in_file_path>  <distance_threshold>\n")
elif any([sys.argv[1] == "-h",
        sys.argv[1] == "--h",
        sys.argv[1] == "-help",
        sys.argv[1] == "--help"]):
    sys.exit("USAGE:\n\n\tTE_insertion_merger.py <full_in_file_path>  <distance_threshold>\n")
elif len(sys.argv) < 3:
    sys.exit("\nNot enough arguments:\nUSAGE:\n\n\tTE_insertion_merger.py <full_in_file_path>  <distance_threshold>\n")

    
insertions = open(sys.argv[1], 'r').readlines()[1:]  ## expects a header

print "\nMMmmmmm thanks. . . . processing . . . \n"

distance_threshold = sys.argv[2]

ins_dict = {}
ins_count = 0
n_merged = 0

for ins in insertions:  ## for each insertion in the file
    
    ins_count +=1 # count the number of insertions processed
    
    ## First make the insertion ID with the format"Strain_Contig_baselocation_strand(+/-)"
    
    ins_site = ins.split()[1]
    ins_ID = "%s_%s_%s_%s" % (ins.split()[4], ins.split()[0], ins.split()[1], ins.split()[3])
    
    ## Then put all of the info into a sub dictionary for this entry
    
    sub_ins_dict = {}
    sub_ins_dict["Contig"]= ins.split()[0]
    sub_ins_dict["Start"]=ins.split()[1]
    sub_ins_dict["End"]=ins.split()[2]
    sub_ins_dict["Strand"]=ins.split()[3]
    sub_ins_dict["Strain"]=ins.split()[4]
    sub_ins_dict["Generation"]=ins.split()[5]
    sub_ins_dict["Family"]=ins.split()[6]
    sub_ins_dict["Ins_type"]=ins.split()[7]
    sub_ins_dict["Program"]=ins.split()[8]
    sub_ins_dict["Evidence"]=ins.split()[9]

    
    ## Now check to see if the strain and gen are in the master dict, and add if not
    
    if sub_ins_dict["Strain"] not in ins_dict:
        ins_dict[sub_ins_dict["Strain"]] = {}
    if sub_ins_dict["Generation"] not in ins_dict[sub_ins_dict["Strain"]]:
        ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]] = {}
    if sub_ins_dict["Contig"] not in ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]]:
        ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]][sub_ins_dict["Contig"]] = {}
    if  sub_ins_dict["Strand"] not in ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]][sub_ins_dict["Contig"]]:
        ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]][sub_ins_dict["Contig"]][sub_ins_dict["Strand"]] = {}
       
    ## Now check to see if this entry matches one that is already recorded (rec) in the dictionary or has an insertion site nearby
    
    
    
    if len(ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]][sub_ins_dict["Contig"]][sub_ins_dict["Strand"]]) > 0:  ## If there is already something in the dictionary (i.e. everything after first iteration)
        
        for rec_ins_site in ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]][sub_ins_dict["Contig"]][sub_ins_dict["Strand"]].keys():  ## for each existing entry check to see if they match for all conditions below...
            
            found = False  ## flag to tell if insertion matched on already recorded
            
            if (int(rec_ins_site)-int(distance_threshold)) <= int(ins_site) <= (int(rec_ins_site)+int(distance_threshold)):  ## if new ins value is +or- a given distance from the already-recorded insertion
                found = True # change flag to true
                    
                ## If the new insertion matched an old insertion, add it to the entry
                
            if found == True:
                n_merged += 1
                ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]][sub_ins_dict["Contig"]][sub_ins_dict["Strand"]][rec_ins_site][ins_ID] = sub_ins_dict # Add to the already recorded insertion 
                    
        ## if insertion was not already recorded, add it to the dictionary
        
        if not found:
            ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]][sub_ins_dict["Contig"]][sub_ins_dict["Strand"]][ins_site] = {}
            ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]][sub_ins_dict["Contig"]][sub_ins_dict["Strand"]][ins_site][ins_ID] = sub_ins_dict

    ## or if the length of the dictionary is not greater than 0, then add the first insertion to start things off
    
    else:
        ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]][sub_ins_dict["Contig"]][sub_ins_dict["Strand"]][ins_site] = {}
        ins_dict[sub_ins_dict["Strain"]][sub_ins_dict["Generation"]][sub_ins_dict["Contig"]][sub_ins_dict["Strand"]][ins_site][ins_ID] = sub_ins_dict
    
    
print "Processed %s insertions" % ins_count  
print "Merged %s insertions" % n_merged


#########################
######  OUTPUT  #########
#########################

## Make output path

prefix = sys.argv[1].rpartition(".")[0]
suffix = sys.argv[1].rpartition(".")[2]

outpath = "%s_merged.%s" % (prefix, suffix)

print "\nOutputting file to %s\n" % outpath

Outfile = open(outpath, 'w')

Outfile.write("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n" % ("Contig", "start", "end", "strand", "strain", "generation", "Family", "insertion","N_programs", "programs", "evidence", "Insertion_sites_(strt-end)"))

Insertion_index = 0
for strain in ins_dict:
    for gen in ins_dict[strain]:
        for contig in ins_dict[strain][gen]:
            for strand in ins_dict[strain][gen][contig]:
                for ins_site in ins_dict[strain][gen][contig][strand]:
                    if len(ins_dict[strain][gen][contig][strand][ins_site]) > 1:
                        programs = []
                        starts_ends = []
                        Evidences = []
                        for sub_insertion in ins_dict[strain][gen][contig][strand][ins_site]:
                            start = ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["Start"]
                            end = ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["End"]
                            ins_type = ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["Ins_type"]
                            programs.append(ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["Program"])
                            family = ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["Family"]
                            Evidences.append(ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["Evidence"])
                            starts_ends.append("(%s-%s)" % (start, end))
                            
                        line = "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n" % (contig,start,end,strand,strain,gen,family, ins_type,len(ins_dict[strain][gen][contig][strand][ins_site]),",".join(programs),",".join(Evidences), ",".join(starts_ends))

                    else:
                        for sub_insertion in ins_dict[strain][gen][contig][strand][ins_site]:
                            start = ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["Start"]
                            end = ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["End"]
                            ins_type = ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["Ins_type"]
                            programs = ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["Program"]
                            family = ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["Family"]
                            Evidences = ins_dict[strain][gen][contig][strand][ins_site][sub_insertion]["Evidence"]
                            starts_ends = "(%s-%s)" % (start, end)
                        
                        line = "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n" % (contig,start,end,strand,strain,gen,family,ins_type,len(ins_dict[strain][gen][contig][strand][ins_site]),programs,Evidences,starts_ends)
                    
                    Outfile.write(line)

Outfile.close()

print "\n ##  All done  ## \n"
