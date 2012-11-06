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
#ifndef __AVERAGE_DISPLACEMENT_HH__
#define __AVERAGE_DISPLACEMENT_HH__

#include <Eigen/Core>

namespace rlfd {
namespace delay {

/**
 * This algorithm computes the delay times based on the method originally described in:
 *
 * M. T. Rosenstein, J. J. Collins, and C. J. De Luca, "Reconstruction expansion
 * as a geometry-based framework for choosing proper delay times," Physica D:
 * Nonlinear Phenomena, vol. 73, no. 1–2, pp. 82–98, May 1994.
 *
 * @param ts The input time series to analyze
 * @param m The embedding dimension
 * @param nlags The number of average displacement points to compute
 * @return A vector of length nlags containing the average displacement measures (the S_m statistic).
 */
Eigen::VectorXd AverageDisplacement(const Eigen::VectorXd& ts, int m, int nlags)
{
  Eigen::VectorXd ad(nlags);

  // Compute for a range of lags
  for (int k = 0; k < nlags; k++) {
    double average_displacement = 0.0;
    for (int i = 0; i < (ts.size() - nlags*(m-1)); i++)
    {
      double sum_squares = 0.0;
      for (int j = 1; j <= (m-1); j++) {
        sum_squares += std::pow(ts[i + j*k] - ts[i], 2);
      }
      average_displacement += std::sqrt(sum_squares);
    }
    ad[k] = average_displacement/((double) ts.size());
  }

  return ad;
}

} // namespace delay
} // namespace rlfd

#endif
