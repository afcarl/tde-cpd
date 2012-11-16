#!/usr/bin/env Rscript 
require(ggplot2)
args <- commandArgs(TRUE)
filename <- args[1]

dance.data <- read.table(filename, col.names=c("label", "x", "y", "theta", "sine"))
dance.data$time <- 1:nrow(dance.data)

dance.plot <- ggplot(dance.data, aes(time, sine)) + geom_line(aes(color=label, group=1))
svg(paste(filename, ".svg", sep=""))
print(dance.plot)
dev.off()
