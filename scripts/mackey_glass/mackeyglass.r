#!/usr/bin/env Rscript

require(deSolve)
require(ggplot2)

mackeyGlass <- function(t, y, parms, tau) {
  tlag <- t - tau

  if (tlag <= 0)
    ylag <- 0.5
  else
    ylag <- lagvalue(tlag)

  dy <- 0.2 * ylag * 1/(1+ylag^10) - 0.1 * y
  list(c(dy))
}

# Generate mackey-glass attractors with different delays
tmax = 500
yinit <- 0.5
tauValues <- c(17, 23, 30, 35)
nmodes = length(tauValues)
attractors <- matrix(0, nmodes, tmax/0.1 + 1)

for(i in 1:nmodes) {
  times <- seq(from = 0, to = tmax, by = 0.1)
  y <- dede(y = yinit, times = times, func = mackeyGlass, parms = NULL, tau = tauValues[i])
  attractors[i,] = y[, 2]
}
# Subsample
#attractors <- attractors[,seq(from = 1, to = ncol(attractors), by = 10)]

# Randomize the duration
nsegments = 15
#durations <- sample(200:300, nsegments)
durations <- rep(ncol(attractors), nsegments)
modes <- sample(1:4, nsegments, replace=TRUE)

switchingSeries <- numeric()
for (i in 1:nsegments) {
 switchingSeries <- c(switchingSeries, attractors[modes[i], 1:durations[i]])
}

# Merge the rows in one vector
attractors.df <- data.frame(y=switchingSeries, time=1:length(switchingSeries))

# Add measurement noise
#noise <- rnorm(length(xseries), mean=0.0, sd=sd(xseries)*0.3)

ggplot(attractors.df, aes(x=time, y=y)) + geom_line() + geom_vline(xintercept=cumsum(durations), color="blue", linetype="longdash")
