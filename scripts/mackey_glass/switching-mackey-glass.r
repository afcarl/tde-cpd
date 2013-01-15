#!/usr/bin/env Rscript

require(deSolve)

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
tmax = 3000
yinit <- 0.5
tauValues <- c(17, 23, 30, 35)
nmodes = length(tauValues)
attractors <- matrix(0, nmodes, tmax/0.1 + 1)

for(i in 1:nmodes) {
  times <- seq(from = 0, to = tmax, by = 0.1)
  y <- dede(y = yinit, times = times, func = mackeyGlass, parms = NULL, tau = tauValues[i])
  attractors[i,] = y[, 2]
}

# Resample
attractors <- attractors[,seq(from = 1, to = ncol(attractors), by = 60)]
print(ncol(attractors))

# Randomize the duration
nsegments = 7
#durations <- sample(ncol(attractors), nsegments)
durations <- rep(ncol(attractors), nsegments)
modes <- sample(1:4, nsegments, replace=TRUE)

switchingSeries <- numeric()
for (i in 1:nsegments) {
 switchingSeries <- c(switchingSeries, attractors[modes[i], 1:durations[i]])
}

# Add measurement noise
noise <- rnorm(length(switchingSeries), mean=0.0, sd=sd(switchingSeries)*0.3)
switchingSeries <- switchingSeries + noise

# Add label column
labels <- rep(modes, durations)

# Output to STDOUT
write.table(data.frame(x=switchingSeries, label=labels), "mack.dat", quote=FALSE, row.names=FALSE, col.name=FALSE, sep="\t")
