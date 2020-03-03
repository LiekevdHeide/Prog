
#Plotarcflows / pathflows
location <- "X:/My Documents/Wegennetwerk/Prog/Experiment1BBest.txt"

timeLength <- read.table(location, skip = 1, nrows = 1, header = TRUE)

resultingPaths <- read.table(location, skip=3, nrows =55, header=FALSE)
resultingPaths

resultingAllFlows <- read.table(location, skip = r + timeLength, header = FALSE)


require(ggplot2)
require(reshape2)
#ggplot(Results, aes(seq(1, 100, by=1))) + 
#  geom_line(aes(y = s, colour = "State"), size=2) + 
#  geom_line(aes(y = a, colour = "Action"), size=1) +
#  geom_point(aes(y=c, colour="Cost"), size=2)
 
time <- seq(0,54, by = 1)

resultingPaths <- cbind(resultingPaths, time)

#+theme_classic() for no grid!
library(plyr)

meltedResultingPaths0 = melt(resultingPaths[,c(1,2,3,4,14)], id = "time")
meltedResultingPaths0$variable <- revalue(meltedResultingPaths0$variable, c("V1"="Path 1", "V2"="Path 2", "V3"="Path 3", "V4"="Path 4"))
ggplot(meltedResultingPaths0, aes(x= time, y = value, group = variable)) + 
  geom_point(aes(shape= variable)) +
  theme_bw()+
  ylab(label = "Recurrent traffic flow on path OD 0") + 
  xlab("Time") + 
  guides(shape = guide_legend(title = "Path flows OD 0")) +
  scale_shape(solid = FALSE) 

meltedResultingPaths1 = melt(resultingPaths[,c(5,6,7,14)], id = "time")
meltedResultingPaths1$variable <- revalue(meltedResultingPaths1$variable, c("V5"="Path 1", "V6"="Path 2", "V7"="Path 3"))
ggplot(meltedResultingPaths1, aes(x= time, y = value, group = variable)) + 
  geom_point(aes(shape= variable)) +
  theme_bw()+
  ylab(label = "Recurrent traffic flow on path OD 1") + 
  xlab("Time") + 
  guides(shape = guide_legend(title = "Path flows OD 1")) +
  scale_shape(solid = FALSE) 

meltedResultingPaths2 = melt(resultingPaths[,c(8,9,10,14)], id = "time")
meltedResultingPaths2$variable <- revalue(meltedResultingPaths2$variable, c("V8"="Path 1", "V9"="Path 2", "V10"="Path 3"))

ggplot(meltedResultingPaths2, aes(x= time, y = value, group = variable)) + 
  geom_point(aes(shape= variable)) +
  theme_bw()+
  ylab(label = "Recurrent traffic flow on path OD 2") + 
  xlab("Time") + 
  guides(shape = guide_legend(title = "Path flows OD 2")) +
  scale_shape(solid = FALSE) 

meltedResultingPaths3 = melt(resultingPaths[,c(11,12,13,14)], id = "time")
meltedResultingPaths3$variable <- revalue(meltedResultingPaths3$variable, c("V11"="Path 1", "V12"="Path 2", "V13"="Path 3"))
ggplot(meltedResultingPaths3, aes(x= time, y = value, group = variable)) + 
  geom_point(aes(shape= variable)) +
  theme_bw()+
  ylab(label = "Recurrent traffic flow on path OD 3") + 
  xlab("Time") + 
  guides(shape = guide_legend(title = "Path flows OD 3")) +
  scale_shape(solid = FALSE) 
  
#plot arc flows





#----------------------------------------------------------------------------------------
location2 <- "X:/My Documents/Wegennetwerk/Prog/Experiment2Best.txt"

timeLength2 <- read.table(location2, skip = 1, nrows = 1, header = TRUE)

resultingPaths2 <- read.table(location2, skip=3, nrows =55, header=FALSE)
resultingPaths2

time <- seq(0,54, by = 1)

resultingPaths2 <- cbind(resultingPaths2, time)

meltedResulting2Paths0 = melt(resultingPaths2[,c(seq(1,12), 34)], id = "time")
meltedResulting2Paths0$variable <- revalue(meltedResulting2Paths0$variable, c("V1"="Path 1", "V2" = "Path 2", "V3" = "Path 3", "V4" = "Path 4", "V5" = "Path 5",
                                                                              "V6" = "Path 6", "V7" = "Path 7", "V8" = "Path 8", "V9" = "Path 9", "V10" = "Path 10",
                                                                              "V11" = "Path 11", "V12" = "Path 12"))
#ggplot(meltedResulting2Paths0, aes(x= time, y = value, group = variable)) + 
#  geom_point(aes(shape= variable)) +
#  theme_bw()+
#  ylab(label = "Recurrent traffic flow on path OD 0") + 
#  xlab("Time") + 
#  guides(shape = guide_legend(title = "Path flows OD 0")) +
#  scale_shape(solid = FALSE) 
ggplot(meltedResulting2Paths0, aes(x= time, y = value, group = variable)) + 
  geom_point(aes(color = variable)) +
  theme_bw()+
  ylab(label = "Recurrent traffic flow on path OD 0") + 
  xlab("Time") + 
  guides(color = guide_legend(title = "Path flows OD 0")) +
  scale_shape(solid = FALSE) 

meltedResulting2Paths1 = melt(resultingPaths2[,c(seq(13,21), 34)], id = "time")
meltedResulting2Paths1$variable <- revalue(meltedResulting2Paths1$variable, c("V13"="Path 1", "V14" = "Path 2", "V15" = "Path 3", "V16" = "Path 4", "V17" = "Path 5",
                                                                              "V18" = "Path 6", "V19" = "Path 7", "V20" = "Path 8", "V21" = "Path 9"))
ggplot(meltedResulting2Paths1, aes(x= time, y = value, group = variable)) + 
  geom_point(aes(color = variable)) +
  theme_bw()+
  ylab(label = "Recurrent traffic flow on path OD 1") + 
  xlab("Time") + 
  guides(color = guide_legend(title = "Path flows OD 1")) +
  scale_shape(solid = FALSE) 

meltedResulting2Paths2 = melt(resultingPaths2[,seq(22,34)], id = "time")
meltedResulting2Paths2$variable <- revalue(meltedResulting2Paths2$variable, c("V22"="Path 1", "V23" = "Path 2", "V24" = "Path 3", "V25" = "Path 4", "V26" = "Path 5",
                                                                              "V27" = "Path 6", "V28" = "Path 7", "V29" = "Path 8", "V30" = "Path 9", "V31" = "Path 10",
                                                                              "V32" = "Path 11", "V33" = "Path 12"))
ggplot(meltedResulting2Paths2, aes(x= time, y = value, group = variable)) + 
  geom_point(aes(color = variable)) +
  theme_bw()+
  ylab(label = "Recurrent traffic flow on path OD 2") + 
  xlab("Time") + 
  guides(color = guide_legend(title = "Path flows OD 2")) +
  scale_shape(solid = FALSE) 


#---------------------------------------------------------------------------------------------
#Print all flows?




