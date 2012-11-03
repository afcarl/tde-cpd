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
#ifndef __UNBIASED_MULTIPLE_AUTOCORRELATION_HH__
#define __UNBIASED_MULTIPLE_AUTOCORRELATION_HH__

#include <Eigen/Core>
#include <rlfd_segmentation/utils/Autocorrelation.hh>

namespace rlfd {

/**
 * This algorithm computes the delay times based on the method originally described in:
 *
 * M. T. Rosenstein, J. J. Collins, and C. J. De Luca, "Reconstruction expansion
 * as a geometry-based framework for choosing proper delay times," Physica D:
 * Nonlinear Phenomena, vol. 73, no. 1–2, pp. 82–98, May 1994. 
 *
 * We use the mutliple unbiased multiple autocorrelation equivalent by: 
 * H. Ma and C. Han, "Selection of Embedding Dimension and Delay Time in Phase
 * Space Reconstruction," Frontiers of Electrical and Electronic Engineering in
 * China, vol. 1, no. 1, pp. 111–114, Jan. 2006. 
 *
 * This allows us to use the FFT-based autocorrelation that runs in O(nlogn).
 *
 * @param ts The input time series to analyze
 * @param m The embedding dimension
 * @param nlags The number of average displacement points to compute 
 * @return A vector of length nlags containing the average displacement measures (the S_m^2 statistic).
 *
 * @FIXME check for boundary issues.
 */
Eigen::VectorXd UnbiasedMultipleAutocorrelation(const Eigen::VectorXd& ts, int m, int nlags=20)
{
  nlags = std::min(nlags, ts.size()-(m-1));

  // Autocorrelation coefficients
  Eigen::VectorXd acf;

  // Compute the autocorrelation for up to (ts.size()+1)/2 lags
  rlfd::utils::Autocorrelation(ts, acf);

  // Compute the average over the square of the displacements for a range of
  // lags for tau. Since we only have access to discrete samples, tau has to be
  // an integer. With the sampling time s, it effectively corresponds to
  // samples at tau*s^-1 of interval.  
  Eigen::VectorXd ad(nlags);

  double E = (1.0/ts.size())*ts.squaredNorm();

  for (int k = 0; k < nlags; k++) {
    // Compute the sum of the autocorrelations for m-1 lags
    double Rxxm = ts.segment(k, m-1).sum();
    ad[k] = 2.0*((double) (m-1))*E - 2.0*Rxxm;
  }

  return ad;
} 

} // namespace rlfd

#endif
