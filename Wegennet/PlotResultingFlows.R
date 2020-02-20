
#Plotarcflows / pathflows
location <- "X:/My Documents/Wegennetwerk/Prog/ResultsPathFlowsExample.txt"
resultingPaths <- read.table(location, skip=1, nrows =50, header=FALSE)
resultingPaths

require(ggplot2)
require(reshape2)
#ggplot(Results, aes(seq(1, 100, by=1))) + 
#  geom_line(aes(y = s, colour = "State"), size=2) + 
#  geom_line(aes(y = a, colour = "Action"), size=1) +
#  geom_point(aes(y=c, colour="Cost"), size=2)

resultingPaths <- cbind(resultingPaths, time)

meltedResultingPaths0 = melt(resultingPaths[,c(1,2,3,4,14)], id = "time")
ggplot(meltedResultingPaths0, aes(x= time, y = value, colour = variable)) +
  geom_point(size = 1) +
  theme_bw() +
  ylab(label = "Recurrent traffic flow on path OD 0") + 
  xlab("Time")

meltedResultingPaths1 = melt(resultingPaths[,c(5,6,7,14)], id = "time")
ggplot(meltedResultingPaths1, aes(x= time, y = value, colour = variable)) +
  geom_point(size = 1) +
  theme_bw()+
  ylab(label = "Recurrent traffic flow on path OD 1") + 
  xlab("Time")

meltedResultingPaths2 = melt(resultingPaths[,c(8,9,10,14)], id = "time")
ggplot(meltedResultingPaths2, aes(x= time, y = value, colour = variable)) +
  geom_point(size = 1) +
  theme_bw() +
  ylab(label = "Recurrent traffic flow on path OD 2") + 
  xlab("Time")

meltedResultingPaths3 = melt(resultingPaths[,c(11,12,13,14)], id = "time")
ggplot(meltedResultingPaths3, aes(x= time, y = value, colour = variable)) +
  geom_point(size = 1) +
  theme_bw()+
  ylab(label = "Recurrent traffic flow on path OD 0") + 
  xlab("Time")
