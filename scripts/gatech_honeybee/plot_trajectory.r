#!/usr/bin/env Rscript
require(ggplot2)
args <- commandArgs(TRUE)
filename <- args[1]

dance.data <- read.table(filename, col.names=c("label", "x", "y", "theta", "sine"))

dance.plot <- ggplot(dance.data, aes(x, y)) + geom_path(aes(color=label, group=1))
svg(paste(filename, "-path.svg", sep=""))
print(dance.plot)
dev.off()
