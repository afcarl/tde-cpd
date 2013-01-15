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

tmax = 3000
yinit = 0.5
times <- seq(from = 0, to = tmax, by = 0.1)
y <- dede(y = yinit, times = times, func = mackeyGlass, parms = NULL, tau = 17)

y <- y[seq(from=1, to=nrow(y), by=100), 2]

cat(y, sep="\n")
