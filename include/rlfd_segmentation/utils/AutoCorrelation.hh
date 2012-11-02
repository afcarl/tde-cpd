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
#include <Eigen/Core>

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
  // FFTW is optimized for powers of 2.
  int n = std::exp2(std::ceil(std::log2(inSeries.size())));

  // Zero-pad so that the length is a power of two. Remove the mean 
  EIGEN_ALIGN16 Eigen::VectorXd series(n);
  series << (inSeries.array() - inSeries.mean()), Eigen::VectorXd::Zero(n - inSeries.size());

  // Negative-frequency amplitudes for real data are the complex conjugate of
  // the positive-frequency amplitudes 
  int nc = (n+1)/2;

  // Use fftw_malloc to ensure 16-bytes alignment.  
  double* out = (double*) fftw_malloc(sizeof(double)*n); 
  double* power_spectrum = (double *) fftw_malloc(sizeof(double)*n); 

  // Compute the Fourier transform of the input time series
  fftw_plan plan_forward = fftw_plan_r2r_1d(n, series.data(), out, FFTW_R2HC, FFTW_ESTIMATE);
  fftw_execute(plan_forward);
  fftw_destroy_plan(plan_forward);

  // Compute the Power Spectral Density (PSD)
  // The PSD is the squares of the absolute values of the DFT amplitudes.
  power_spectrum[0] = (out[0]*out[0]);  // DC component
  for (int k = 1; k < nc; k++) { 
    // The DFT output satisfies the “Hermitian” redundancy: out[i] is the
    // conjugate of out[n-i]
    power_spectrum[k] = (out[k]*out[k] + out[n-k]*out[n-k]);
  }
  // Nyquist freq.
  if (n % 2 == 0) {
    power_spectrum[n/2] = (out[n/2]*out[n/2]);
  }

  // Set imaginary part to 0 in preparation to ifft
  for (int k = nc; k < n; k++) {
    power_spectrum[k] = 0;
  }

  // By the Wiener–Khinchin theorem, the power spectral density of a
  // wide-sense-stationary random process is the Fourier transform of the
  // autocorrelation function.
  fftw_plan plan_backward = fftw_plan_r2r_1d(n, power_spectrum, out, FFTW_HC2R, FFTW_ESTIMATE);
  fftw_execute(plan_backward);
  fftw_destroy_plan(plan_backward);
  fftw_cleanup();
 
  // RFFTW transforms are unnormalized. Applying the forward and then the
  // backward transform will multiply the input by n.
  outCoeff = Eigen::VectorXd::Map(out, nc).array()*(1.0/n);

  // Normalize the ACF as in Matlab, from Box, Jenkins, Reinsel, pages 30-34, 188. 
  outCoeff = outCoeff.array() * (1.0/outCoeff[0]);

  fftw_free(out);
  fftw_free(power_spectrum);
}

} // namespace utils
} // namespace rlfd

#endif
