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
  list(dy = dy, ylag = ylag)
}


# Generate mackey-glass attractors with different modes
yinit <- 0.5
tauValues <- sample(c(17, 23, 30, 35))
durations <- sample(200:300, 4)

xseries <- numeric()
for(i in 1:length(tauValues)) {
  times <- seq(from = 0, to = durations[i], by = 0.1)

  x <- dede(y = yinit, times = times, func = mackeyGlass, parms = NULL, tau = tauValues[i])
  xseries <- c(xseries, x[, 2])
}

# Add measurement noise
noise <- rnorm(length(xseries), mean=0.0, sd=sd(xseries)*0.3)
xseries = xseries #+ noise

# Plot
plot(xseries, type="l")
