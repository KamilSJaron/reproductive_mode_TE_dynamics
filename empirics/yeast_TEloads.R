#Yeast insertion data

library(ggplot2)
library(plyr)
library(scales)
library(lattice)
library(lme4)
library(forecast)
library(gridExtra)
library(ggpubr)


anovsF3<-function(reponse,v1,v2,v3,nbrep=5000){
  rep<-reponse
  var1<-v1
  var2<-v2
  var3<-v3
  data<-data.frame(rep=rep,var1=var1,var2=var2,var3=var3)
  obs<-data.frame(t(anova(lm(rep~var1+var2*var3,data=data))$F))
  names(obs)<-c("Factor 1","Factor 2","Factor 3","Interaction","")
  for (i in 2:nbrep) {
    obs[i,]<-t(anova(lm(sample(rep)~var1+var2*var3,data=data))$F)
  }
  pval<-NULL
  for (j in 1:4) {
    pval[j]<-sum(obs[,j]>=obs[1,j])/length(obs[,j])
  }
  par(mfrow=c(4,1))
  hist(obs$"Factor 1",xlab="F");abline(v=obs[1,1],lwd=2)
  hist(obs$"Factor 2",xlab="F");abline(v=obs[1,2],lwd=2)
  hist(obs$"Factor 3",xlab="F");abline(v=obs[1,3],lwd=2)
  hist(obs$"Interaction",xlab="F");abline(v=obs[1,4],lwd=2)
  
  return(pval)
}


anovsF2<-function(reponse,v1,v2,nbrep=5000){
  rep<-reponse
  var1<-v1
  var2<-v2
  data<-data.frame(rep=rep,var1=var1,var2=var2)
  obs<-data.frame(t(anova(lm(rep~var1*var2,data=data))$F))
  names(obs)<-c("Factor 1","Factor 2","Interaction","")
  for (i in 2:nbrep) {
    obs[i,]<-t(anova(lm(sample(rep)~var1*var2,data=data))$F)
  }
  pval<-NULL
  for (j in 1:3) {
    pval[j]<-sum(obs[,j]>=obs[1,j])/length(obs[,j])
  }
  par(mfrow=c(4,1))
  hist(obs$"Factor 1",xlab="F");abline(v=obs[1,1],lwd=2)
  hist(obs$"Factor 2",xlab="F");abline(v=obs[1,2],lwd=2)
  hist(obs$"Interaction",xlab="F");abline(v=obs[1,3],lwd=2)
  
  return(pval)
}



#specify names and mode of repr
names_asex <- c("3C","3D","6C","6D")
match_asex <- paste(names_asex, collapse="|")
names_sex <- c("2A-5A","2C-5C","2D-5D","2F-5F")
match_sex <- paste(names_sex, collapse="|")


##################################################
# TE fraq cov from bam files used for TEs bergman#
##################################################

TECov = read.table(file="~/Dropbox/UNIL/yeast/coverage_from_bam/read_stats.txt", sep="\t", header=T)

#remove 3D-90 because of low coverage
TECov <-subset(TECov, subset=!(strain=="3D"&generation=="90"))

#add repr mode info in new column
TECov$mode <- ifelse(grepl(match_asex,TECov$strain), "asex",
                     ifelse(grepl(match_sex,TECov$strain), "sex","Other"))
TECov$mode <- as.factor(TECov$mode)


empiriplot1 <- ggplot(TECov, aes(TECov$generation, TECov$cov, color=TECov$mode, group=TECov$mode)) + geom_point(size=3, alpha=0.8) +
  geom_smooth(method=lm, se=T, fill="lightgrey") +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 6), size = 1) +
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="fraction of reads mapping to TEs") +
  coord_cartesian(ylim=c(0,0.035)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")+
  theme(plot.margin=unit(c(40,20,10,10), "pt"))

#effect size
round((1-(mean(TECov$cov[TECov$mode=="asex" & TECov$generation=="990"]) / mean(TECov$cov[TECov$mode=="asex" & TECov$generation=="0"] ))) *100, 2)
#asexuals 23.5% less TE load

#ANOVA
#anovsF2(TECov$cov, TECov$generation, TECov$mode)
#[1] 0.0002 0.0814 0.0002


########################################
# McClintock TE abundances preparation #
########################################

#here, only direct evidence for presence // and with the artificial merged and split 20bases removed from each read "paired" reads:
TEevidence = read.table(file="~/Dropbox/UNIL/yeast/mcclintock/PAIRED/collected_nonredundant_results_no_nonab_merged.tbl3", sep="\t", header=T)

#for subsampled mean asex every generation
#TEevidence = read.table(file="~/Dropbox/UNIL/yeast/mcclintock/subsample/collected_results/collected_nonredundant_results_no_nonab_merged.tbl3", sep="\t", header=T)

#remove 3D-90 because of low coverage
TEevidence <-subset(TEevidence, subset=!(strain=="3D"&generation=="90"))

#add count
TEevidence$count <- "1"

#add repr mode info in new column
TEevidence$mode <- ifelse(grepl(match_asex,TEevidence$strain), "asex",
                          ifelse(grepl(match_sex,TEevidence$strain), "sex","Other"))

TEevidence$mode <- as.factor(TEevidence$mode)
TEevidence$count <- as.numeric(TEevidence$count)

#add delta length of insertion
TEevidence$size <- TEevidence$endsite-TEevidence$startsite

########
#FILTER#
########
#remove everything with lower size then XXX
#TEevidence <- subset(TEevidence, size>=20)

#Assign labels
TEevidence$type <- ifelse((grepl("non-reference", TEevidence$insertion)), "non-ref",
                          ifelse((grepl("TY1", TEevidence$Family) & TEevidence$size>=5425 & TEevidence$size<=6425), "full",
                                 ifelse((grepl("TY2", TEevidence$Family) & TEevidence$size>=5458 & TEevidence$size<=6458), "full",
                                        ifelse((grepl("TY3", TEevidence$Family) & TEevidence$size>=4851 & TEevidence$size<=5851), "full",
                                               ifelse((grepl("TY3_1p", TEevidence$Family) & TEevidence$size>=4905 & TEevidence$size<=5905), "full",
                                                      ifelse((grepl("TY4", TEevidence$Family) & TEevidence$size>=5726 & TEevidence$size<=6726), "full",
                                                             ifelse((grepl("TY5", TEevidence$Family) & TEevidence$size>=4876 & TEevidence$size<=5876), "full",
                                                                    ifelse((TEevidence$size>220 & TEevidence$size<420), "soloLTR","other"))))))))

#TEevidence$type2 <- ifelse((grepl("non-reference", TEevidence$insertion)), "non-ref",
#                           ifelse((grepl("reference", TEevidence$insertion)), "ref","other"))


TEevidence_fullTE <- subset(TEevidence, type=="full")
TEevidence_fullTE_nonref <- subset(TEevidence, type=="full" |  type=="non-ref")
TEevidence_nonref <- subset(TEevidence, type=="non-ref")
TEevidence_ref <- subset(TEevidence, insertion=="reference")

#################################################
# sample cov from bam files used for TEs bergman#
#################################################

SampleCovMedian = read.table(file="~/Dropbox/UNIL/yeast/mcclintock/PAIRED/cov.all.median", sep=" ", header=T)

#add repr mode info in new column
SampleCovMedian$mode <- ifelse(grepl(match_asex,SampleCovMedian$strain), "asex",
                               ifelse(grepl(match_sex,SampleCovMedian$strain), "sex","Other"))
SampleCovMedian$mode <- as.factor(SampleCovMedian$mode)
str(SampleCovMedian)

summary(SampleCovMedian$cov[SampleCovMedian$mode=="asex"])
summary(SampleCovMedian$cov[SampleCovMedian$mode=="sex"])

covplot <- ggplot(SampleCovMedian, aes(SampleCovMedian$mode, SampleCovMedian$cov, fill=SampleCovMedian$mode)) + 
  geom_boxplot(alpha=0.8) +
  scale_fill_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="median read coverage per sample") +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none") +
  theme(plot.margin=unit(c(10,20,10,10), "pt"))

#remove coverage 0 sample
SampleCovMedian <-subset(SampleCovMedian, subset=!(strain=="3D"&generation=="90"))



###########
#TE counts#
###########

#sum the TE insertions by strain and generation
#aggregate(x$Frequency, by=list(Category=x$Category), FUN=sum)

#for all with >=1 program support
TEevidence_counts <- as.data.frame(aggregate(TEevidence$count, by=list(strain=TEevidence$strain, generation=TEevidence$generation), FUN=sum))

#for all reference TEs
TEevidence_ref_counts <- as.data.frame(aggregate(TEevidence_ref$count, by=list(strain=TEevidence_ref$strain, generation=TEevidence_ref$generation), FUN=sum))

#solo and full length
TEevidence_fullTE_counts <- as.data.frame(aggregate(TEevidence_fullTE$count, by=list(strain=TEevidence_fullTE$strain, generation=TEevidence_fullTE$generation), FUN=sum))
#TEevidence_fullTE_Familycounts <- as.data.frame(aggregate(TEevidence_fullTE$count, by=list(family=TEevidence_fullTE$Family), FUN=sum))

TEevidence_fullTE_nonref_counts <- as.data.frame(aggregate(TEevidence_fullTE_nonref$count, by=list(strain=TEevidence_fullTE_nonref$strain, generation=TEevidence_fullTE_nonref$generation), FUN=sum))

#nonref
TEevidence_nonref_counts <- as.data.frame(aggregate(TEevidence_nonref$count, by=list(strain=TEevidence_nonref$strain, generation=TEevidence_nonref$generation), FUN=sum))


#add repr mode info in new column
TEevidence_counts$mode <- ifelse(grepl(match_asex,TEevidence_counts$strain), "asex",
                                 ifelse(grepl(match_sex,TEevidence_counts$strain), "sex","Other"))

TEevidence_fullTE_counts$mode <- ifelse(grepl(match_asex,TEevidence_fullTE_counts$strain), "asex",
                                        ifelse(grepl(match_sex,TEevidence_fullTE_counts$strain), "sex","Other"))

TEevidence_fullTE_nonref_counts$mode <- ifelse(grepl(match_asex,TEevidence_fullTE_nonref_counts$strain), "asex",
                                        ifelse(grepl(match_sex,TEevidence_fullTE_nonref_counts$strain), "sex","Other"))

TEevidence_nonref_counts$mode <- ifelse(grepl(match_asex,TEevidence_nonref_counts$strain), "asex",
                                               ifelse(grepl(match_sex,TEevidence_nonref_counts$strain), "sex","Other"))

TEevidence_ref_counts$mode <- ifelse(grepl(match_asex,TEevidence_ref_counts$strain), "asex",
                                        ifelse(grepl(match_sex,TEevidence_ref_counts$strain), "sex","Other"))

#rename header counts
TEevidence_counts <- rename(TEevidence_counts, c("x"="counts"))
TEevidence_fullTE_counts <- rename(TEevidence_fullTE_counts, c("x"="counts"))
TEevidence_fullTE_nonref_counts <- rename(TEevidence_fullTE_nonref_counts, c("x"="counts"))
TEevidence_nonref_counts <- rename(TEevidence_nonref_counts, c("x"="counts"))
TEevidence_ref_counts <- rename(TEevidence_ref_counts, c("x"="counts"))

#add coverage info
TEevidence_counts <- merge(SampleCovMedian,TEevidence_counts,by=c("strain","generation","mode" ))
TEevidence_fullTE_counts <- merge(SampleCovMedian,TEevidence_fullTE_counts,by=c("strain","generation","mode" ))
TEevidence_fullTE_nonref_counts <- merge(SampleCovMedian,TEevidence_fullTE_nonref_counts,by=c("strain","generation","mode" ))
TEevidence_nonref_counts <- merge(SampleCovMedian,TEevidence_nonref_counts,by=c("strain","generation","mode" ))
TEevidence_ref_counts <- merge(SampleCovMedian,TEevidence_ref_counts,by=c("strain","generation","mode" ))

#convert as factor
TEevidence_counts$mode <- as.factor(TEevidence_counts$mode)
TEevidence_fullTE_counts$mode <- as.factor(TEevidence_fullTE_counts$mode)
TEevidence_fullTE_nonref_counts$mode <- as.factor(TEevidence_fullTE_nonref_counts$mode)
TEevidence_nonref_counts$mode <- as.factor(TEevidence_nonref_counts$mode)
TEevidence_ref_counts$mode <- as.factor(TEevidence_ref_counts$mode)


###############
#TE insertions#
###############

#TE inserts at least 1 method support
#reference+nonreference inserts
ggplot(TEevidence_counts, aes(TEevidence_counts$generation, TEevidence_counts$counts, color=TEevidence_counts$mode, group=TEevidence_counts$mode)) + geom_point(size=4, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 6), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="count of all reference and non-reference insertions") +
  #coord_cartesian(ylim=c(0,500)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")

#stats
#anovsF3(TEevidence_counts$counts, TEevidence_counts$cov, TEevidence_counts$generation, TEevidence_counts$mode)
#including all TEs
#0.0002 0.0086 0.0402 0.0064

#how many TE could be detected at the starting point (and min max)?
summary(TEevidence_counts$counts)
summary(TEevidence_counts$counts[TEevidence_counts$mode=="asex"])
summary(TEevidence_counts$counts[TEevidence_counts$mode=="sex"])
summary(TEevidence_counts$counts[TEevidence_counts$mode=="asex" & TEevidence_counts$generation=="990"])
summary(TEevidence_counts$counts[TEevidence_counts$mode=="sex" & TEevidence_counts$generation=="990"])
summary(TEevidence_counts$counts[TEevidence_counts$generation=="0"])


#we know coverage has an effect, so we only plot residuals (stats ok though)
TEevidence_counts$resid <- resid(lm(counts~cov, data=TEevidence_counts))

ggplot(TEevidence_counts, aes(TEevidence_counts$generation, TEevidence_counts$resid, color=TEevidence_counts$mode, group=TEevidence_counts$mode)) + geom_point(size=4, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 6), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="count of all ref and non-ref insertions (residuals)") +
  #coord_cartesian(ylim=c(0,500)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")

#effect size
#get slope for effect size
m2 <- lm(counts~cov, data=TEevidence_counts[TEevidence_counts$mode=="asex", ])
summary(lm(resid(m2)~generation, data=TEevidence_counts[TEevidence_counts$mode=="asex", ]))
#slope: -0.11462 // *1000 generations = -114.6
#slope: -0.12099 // *1000 generations = -120.9


###########################################
#full TE all insertions (refins+nonrefins)#
###########################################
ggplot(TEevidence_fullTE_counts, aes(TEevidence_fullTE_counts$generation, TEevidence_fullTE_counts$counts, color=TEevidence_fullTE_counts$mode, group=TEevidence_fullTE_counts$mode)) + geom_point(size=4, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 4), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="full TE count ref and non-ref insertions") +
  coord_cartesian(ylim=c(0,50)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")

subsampleFULL <- ggplot(TEevidence_fullTE_counts, aes(TEevidence_fullTE_counts$generation, TEevidence_fullTE_counts$counts, color=TEevidence_fullTE_counts$mode, group=TEevidence_fullTE_counts$mode)) + geom_point(size=4, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 4), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="empirical full-length TE count") +
  coord_cartesian(ylim=c(0,50)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")
#slopes for subsampling
#mSUB <- (lm(counts~generation, data=TEevidence_fullTE_counts[TEevidence_fullTE_counts$mode=="asex", ]))
#summary(m3)
#-0.01239 * 1000 generations = -12.4

#stats
#anovsF3(TEevidence_fullTE_counts$counts, TEevidence_fullTE_counts$cov, TEevidence_fullTE_counts$generation, TEevidence_fullTE_counts$mode)
#[1] 0.0002 0.0064 0.0334 0.0008 (with +-200bp per TE)
#for subsampling
#anovsF2(TEevidence_fullTE_counts$counts, TEevidence_fullTE_counts$generation, TEevidence_fullTE_counts$mode)
#[1] 0.0124 0.3020 0.0040 (for +-200 bp per TE)

#Full and non-ref TE counts both
ggplot(TEevidence_fullTE_nonref_counts, aes(TEevidence_fullTE_nonref_counts$generation, TEevidence_fullTE_nonref_counts$counts, color=TEevidence_fullTE_nonref_counts$mode, group=TEevidence_fullTE_nonref_counts$mode)) + geom_point(size=4, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 4), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="full TE count ref and non-ref insertions") +
  #coord_cartesian(ylim=c(0,50)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")


#how many TE could be detected at the starting point (and min max)?
summary(TEevidence_fullTE_counts$counts)
summary(TEevidence_fullTE_counts$counts[TEevidence_fullTE_counts$mode=="asex"])
summary(TEevidence_fullTE_counts$counts[TEevidence_fullTE_counts$mode=="sex"])
summary(TEevidence_fullTE_counts$counts[TEevidence_fullTE_counts$mode=="asex" & TEevidence_fullTE_counts$generation=="990"])
summary(TEevidence_fullTE_counts$counts[TEevidence_fullTE_counts$mode=="sex" & TEevidence_fullTE_counts$generation=="990"])
summary(TEevidence_fullTE_counts$counts[TEevidence_fullTE_counts$generation=="0"])


#we know coverage has an effect, so we only plot residuals (stats ok though)
TEevidence_fullTE_counts$resid <- resid(lm(counts~cov, data=TEevidence_fullTE_counts))
TEevidence_fullTE_nonref_counts$resid <- resid(lm(counts~cov, data=TEevidence_fullTE_nonref_counts))


#full TE all insertions resid (refins+nonrefins)
empiriplot2 <- ggplot(TEevidence_fullTE_counts, aes(TEevidence_fullTE_counts$generation, TEevidence_fullTE_counts$resid, color=TEevidence_fullTE_counts$mode, group=TEevidence_fullTE_counts$mode)) + geom_point(size=3, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 4), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="empirical full-length TE count (residuals)") +
  coord_cartesian(ylim=c(-40,12)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")+
  theme(plot.margin=unit(c(40,20,10,10), "pt"))

#effect size
#get slope for effect size
m2 <- lm(counts~cov, data=TEevidence_fullTE_counts[TEevidence_fullTE_counts$mode=="asex", ])
m3 <- (lm(resid(m2)~generation, data=TEevidence_fullTE_counts[TEevidence_fullTE_counts$mode=="asex", ]))
summary(m3)
#slope: -0.008873 // *1000 generations = -8.87


#sex not necessary, because we know it must be zero because of stats

#this is to get the CI values for the TE losses
#confint(m3, level=0.95)
#CI: -0.002883387 * 1000 = 2.88 -> -3 copies
#CI: -0.0148634 = 14.8 -> -15 copies

#this is specifically for object inheriting form "lm"
predict.lm(m3, interval = 'confidence')
predict.lm(m3, interval = 'confidence', se.fit = T)
?predict.lm

#Full and non-ref TE counts both residuals
ggplot(TEevidence_fullTE_nonref_counts, aes(TEevidence_fullTE_nonref_counts$generation, TEevidence_fullTE_nonref_counts$resid, color=TEevidence_fullTE_nonref_counts$mode, group=TEevidence_fullTE_nonref_counts$mode)) + geom_point(size=3, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 4), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="empirical full-length TE count (residuals)") +
  #coord_cartesian(ylim=c(-40,12)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")+
  theme(plot.margin=unit(c(40,20,10,10), "pt"))

#effect size
#get slope for effect size
m2 <- lm(counts~cov, data=TEevidence_fullTE_nonref_counts[TEevidence_fullTE_nonref_counts$mode=="asex", ])
m3 <- (lm(resid(m2)~generation, data=TEevidence_fullTE_nonref_counts[TEevidence_fullTE_nonref_counts$mode=="asex", ]))
summary(m3)
#slope: -0.02253 // *1000 generations = -22.53


#######################
#TE non-ref insertions#
#######################
#exclusively non-ref insertions

ggplot(TEevidence_nonref_counts, aes(TEevidence_nonref_counts$generation, TEevidence_nonref_counts$counts, color=TEevidence_nonref_counts$mode, group=TEevidence_nonref_counts$mode)) + geom_point(size=4, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 4), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="count of non-reference insertions") +
  #coord_cartesian(ylim=c(0,50)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")

#stats
#anovsF3(TEevidence_nonref_counts$counts, TEevidence_nonref_counts$cov, TEevidence_nonref_counts$generation, TEevidence_nonref_counts$mode)
#[1] 0.0002 0.3380 0.2714 0.5992 #no difference
#dev.off()

#we know coverage has an effect, so we only plot residuals (stats ok though)
TEevidence_nonref_counts$resid <- resid(lm(counts~cov, data=TEevidence_nonref_counts))


plot_nonref <- ggplot(TEevidence_nonref_counts, aes(TEevidence_nonref_counts$generation, TEevidence_nonref_counts$resid, color=TEevidence_nonref_counts$mode, group=TEevidence_nonref_counts$mode)) + geom_point(size=3, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 4), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="empirical non-reference TE count (residuals)") +
  coord_cartesian(ylim=c(-100,100)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")+
  theme(plot.margin=unit(c(40,20,10,10), "pt"))

#effect size
#get slope for effect size
m2 <- lm(counts~cov, data=TEevidence_nonref_counts[TEevidence_nonref_counts$mode=="asex", ])
m3 <- (lm(resid(m2)~generation, data=TEevidence_nonref_counts[TEevidence_nonref_counts$mode=="asex", ]))
summary(m3)
#slope: -0.01239 // *1000 generations = -12.39

#######################
#TE ref insertions#
#######################
#exclusively ref insertions

ggplot(TEevidence_ref_counts, aes(TEevidence_ref_counts$generation, TEevidence_ref_counts$counts, color=TEevidence_ref_counts$mode, group=TEevidence_ref_counts$mode)) + geom_point(size=4, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 4), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="reference insertions count") +
  #coord_cartesian(ylim=c(0,50)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")

#stats
#anovsF3(TEevidence_ref_counts$counts, TEevidence_ref_counts$cov, TEevidence_ref_counts$generation, TEevidence_ref_counts$mode)
#[1] 0.0002 0.0022 0.0272 0.0002
#dev.off()

#we know coverage has an effect, so we only plot residuals (stats ok though)
TEevidence_ref_counts$resid <- resid(lm(counts~cov, data=TEevidence_ref_counts))


plot_ref <- ggplot(TEevidence_ref_counts, aes(TEevidence_ref_counts$generation, TEevidence_ref_counts$resid, color=TEevidence_ref_counts$mode, group=TEevidence_ref_counts$mode)) + geom_point(size=3, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 4), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="empirical reference TE count (residuals)") +
  coord_cartesian(ylim=c(-100,100)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none")+
  theme(plot.margin=unit(c(40,20,10,10), "pt"))

#effect size
#get slope for effect size
m2 <- lm(counts~cov, data=TEevidence_ref_counts[TEevidence_ref_counts$mode=="asex", ])
m3 <- (lm(resid(m2)~generation, data=TEevidence_ref_counts[TEevidence_ref_counts$mode=="asex", ]))
summary(m3)
#slope: -0.10729 // *1000 generations = -107.2


###########################
#TE insertions vs coverage#
###########################

SampleCovTEevidencecounts <- merge(SampleCovMedian,TEevidence_counts,by=c("strain","generation","mode", "cov" ))
SampleCovFullTEcounts <- merge(SampleCovMedian,TEevidence_fullTE_counts,by=c("strain","generation","mode", "cov" ))

#anovsF2(SampleCovTEevidencecounts$cov, SampleCovTEevidencecounts$generation, SampleCovTEevidencecounts$mode)
#[1] 0.0960 0.0016 0.5880

#anovsF2(SampleCovTEevidencecounts$counts, SampleCovTEevidencecounts$cov, SampleCovTEevidencecounts$mode)
#[1] 2e-04 5e-03 2e-04
#[1] 0.0002 0.0268 0.0002
#dev.off()

covinsplot <- ggplot(SampleCovTEevidencecounts, aes(SampleCovTEevidencecounts$cov, SampleCovTEevidencecounts$counts)) + geom_point(size=3, alpha=0.8, colour="grey30") +
  geom_smooth(method="nls",formula=y~1+Vmax*(1-exp(-x/tau)),method.args = list(start=c(tau=1,Vmax=2)), se=F, colour="grey30") +
  labs(x="median read coverage per sample", y="number of ref and nonref insertions") +
  #coord_cartesian(ylim=c(0,450)) +
  #scale_x_continuous(trans=log10_trans()) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none") #+
  #theme(plot.margin=unit(c(10,20,10,10), "pt"))



#through generations

covplot2 <- ggplot(SampleCovTEevidencecounts, aes(SampleCovTEevidencecounts$generation, SampleCovTEevidencecounts$cov, color=SampleCovTEevidencecounts$mode, group=SampleCovTEevidencecounts$mode)) + geom_point(size=3, alpha=0.8) +
  #geom_smooth(method=lm, se=T, formula = y ~ poly(x, 4), size = 1) +
  geom_smooth(method=lm, se=T, fill="lightgrey") + 
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="median read coverage per sample") +
  #coord_cartesian(ylim=c(-40,12)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none") #+
  #theme(plot.margin=unit(c(40,20,10,10), "pt"))


######################
#plotting simulations#
######################

#TEsims = read.table(file="~/Dropbox/UNIL/yeast/simulations/overall_sims2.csv", sep="\t", header=T)
TEsims = read.table(file="~/Dropbox/UNIL/yeast/simulations/overall_sims.csv", sep="\t", header=T)
TEsimsmei = read.table(file="~/Dropbox/UNIL/yeast/simulations/overall_sims_meiosis.csv", sep="\t", header=T)
TEsimsmeisex = read.table(file="~/Dropbox/UNIL/yeast/simulations/overall_sims_meiosis_sex.csv", sep="\t", header=T)
TEsimsmeiasex = read.table(file="~/Dropbox/UNIL/yeast/simulations/overall_sims_meiosis_asex.csv", sep="\t", header=T)

TEsimsmei$meioticrate <- as.factor(TEsimsmei$meioticrate)
TEsimsmeisex$meioticrate <- as.factor(TEsimsmeisex$meioticrate)
TEsimsmeiasex$meioticrate <- as.factor(TEsimsmeiasex$meioticrate)


simplot <- ggplot(TEsims, aes(TEsims$generation, TEsims$tebeforesex, color=TEsims$reprmode, group=interaction(reprmode,replicate))) +
  #geom_point(size=0.5, alpha=0.8) +
  geom_line(size=0.5, alpha=0.8) +
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="simulated full-length TE count") +
  coord_cartesian(ylim=c(0,60)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none") +
  theme(plot.margin=unit(c(40,20,10,10), "pt"))

simplot2 <- ggplot(TEsims, aes(TEsims$generation, TEsims$tebeforesex, color=TEsims$reprmode, group=interaction(reprmode,replicate))) +
  geom_line(size=0.7, alpha=0.8, linetype="dotted") +
  #geom_point(size=0.5, alpha=0.8) +
  geom_smooth(aes( group = reprmode ), method="lm", se=T, fill="lightgrey") +
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="simulated full-length TE count") +
  coord_cartesian(ylim=c(0,60)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none") +
  theme(plot.margin=unit(c(40,20,10,10), "pt"))

ggplot(TEsims, aes(TEsims$generation, TEsims$modfreq, color=TEsims$reprmode, group=interaction(reprmode,replicate))) +
  #geom_point(size=0.5, alpha=0.8) +
  geom_line(size=0.5, alpha=0.8) +
  scale_color_manual(values=c('skyblue4', 'orangered2')) +
  labs(x="generation", y="frequency of modifier allele") +
  coord_cartesian(ylim=c(0,1)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position="none") +
  theme(plot.margin=unit(c(40,20,10,10), "pt"))


#Simulations with different meiotic transposition rates

Simsexplot <- ggplot(TEsimsmeisex, aes(TEsimsmeisex$generation, TEsimsmeisex$tebeforesex, color=TEsimsmeisex$meioticrate, group=interaction(replicate,meioticrate))) +
  geom_line(size=0.5, alpha=0.8) +
  #geom_line(size=0.5, alpha=0.8, linetype="dotted") +
  #geom_smooth(aes( group = meioticrate ), method="lm", se=T, fill="lightgrey") +
  #scale_color_brewer(name="Meiotic transposition rate", palette="Reds") +
  scale_color_manual(name="Meiotic transposition rate", values=c('orange1', 'orange3','orangered2','orangered3','orangered4', 'skyblue4')) +
  labs(x="generation", y="simulated full-length TE count") +
  coord_cartesian(ylim=c(0,60)) +
  theme(axis.title = element_text(family = "Arial", color="#666666", face="plain", size=18), axis.text.x = element_text(family = "Arial", color="#666666", face="plain", size=14), axis.text.y = element_text(family = "Arial", color="#666666", face="plain", size=14)) +
  theme(panel.border = element_rect(linetype = "solid", colour = "grey", fill = NA), panel.grid.major = element_line(color = "grey", linetype = "dotted"), panel.grid.minor = element_line(colour = "grey", linetype = "dotted"), panel.background = element_blank(), axis.line = element_line(colour = "grey40")) +
  theme(legend.position=c(0.2,0.25), legend.text = element_text(family = "Arial", color="#666666", face="plain", size=12), legend.title = element_text(family = "Arial", color="#666666", face="plain", size=12)) +
  theme(plot.margin=unit(c(40,20,10,10), "pt"))
Simsexplot + geom_line(data=TEsimsmeiasex, aes(TEsimsmeiasex$generation, TEsimsmeiasex$tebeforesex, color=TEsimsmeiasex$reprmode, group=interaction(reprmode, replicate))) 
  


grid.arrange(empiriplot2, simplot, nrow = 1)

ggarrange(empiriplot1, empiriplot2, simplot, 
          labels = c("A", "B","C"),
          ncol = 1, nrow = 3)
ggarrange(empiriplot1, simplot, 
          labels = c("A", "B"),
          ncol = 2, nrow = 1)
ggarrange(empiriplot1, simplot2, 
          labels = c("A", "B"),
          ncol = 2, nrow = 1)
ggarrange(empiriplot2, simplot2, 
          labels = c("A", "B"),
          ncol = 2, nrow = 1)

ggarrange(plot_ref, plot_nonref, 
          labels = c("A", "B"),
          ncol = 2, nrow = 1)

ggarrange(covinsplot, covplot2, 
          labels = c("A", "B"),
          ncol = 2, nrow = 1)

ggarrange(covinsplot, covplot2, subsampleFULL, 
          labels = c("A", "B", "C"),
          ncol = 2, nrow = 2)

dev.off()
