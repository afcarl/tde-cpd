/**
 * Skills segmentation and learning for Robot Learning by Demonstration
 * Copyright (C) 2012  Pierre-Luc Bacon <pierre-luc.bacon@mail.mcgill.ca> 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */
#ifndef __AUTOCORRELATION_HH__
#define __AUTOCORRELATION_HH__

#include <fftw3.h>

namespace rlfd {
namespace utils {

/**
 * Compute the sample autocorrelation function for the specified lag.
 * @param ts The time series data
 * @param lag The autocorrelation lag
 * @param outCoeff Output vector for holding the autocorrelation coefficients
 * @param nlags The number of lags to compute
 */
void AutoCorrelation(const Eigen::VectorXd& inSeries, Eigen::VectorXd& outCoeff, int nlags=20)
{
  // Remove the mean 
  EIGEN_ALIGN16 Eigen::VectorXd series = (inSeries.array() - inSeries.mean());


  // FFTW is optimized for powers of 2.
  int n = std::exp2(std::ceil(std::log2(series.size())));
  // Half-complex format
  int nc = (n+1)/2;

  // In the backward phase, the output will be multiplied by the number of elements
  // in the input vector. Use fftw_malloc to ensure 16-bytes alignment.  
  double scale = (1.0/nc);
  double* out = (double*) fftw_malloc(sizeof(double)*n); 
  double* power_spectrum = (double *) fftw_malloc(sizeof(double)*nc); 

  // Compute the Fourier transform of the input time series
  fftw_plan plan_forward = fftw_plan_r2r_1d(n, series.data(), out, FFTW_R2HC, FFTW_ESTIMATE);
  fftw_execute(plan_forward);
  fftw_destroy_plan(plan_forward);

  // Compute the Power Spectral Density (PSD)
  // The PSD is the squares of the absolute values of the DFT amplitudes.
  power_spectrum[0] = (out[0]*out[0])*scale;  // DC component
  
  for (int k = 1; k < nc; k++) { 
    // The DFT output satisfies the “Hermitian” redundancy: out[i] is the
    // conjugate of out[n-i]
    power_spectrum[k] = (out[k]*out[k] + out[n-k]*out[n-k])*scale;
  }

  // Nyquist freq.
  if (n % 2 == 0) {
    power_spectrum[n/2] = (out[n/2]*out[n/2])*scale;
  }

  // By the Wiener–Khinchin theorem, the power spectral density of a
  // wide-sense-stationary random process is the Fourier transform of the
  // autocorrelation function.
  fftw_plan plan_backward = fftw_plan_r2r_1d(nc, power_spectrum, out, FFTW_HC2R, FFTW_ESTIMATE);
  fftw_execute(plan_backward);
  fftw_destroy_plan(plan_backward);
 
  outCoeff = (Eigen::VectorXd::Map(out, nc).array() * (1.0/out[0]));
  fftw_free(out);
  fftw_free(power_spectrum);
}

} // namespace utils
} // namespace rlfd

#endif
