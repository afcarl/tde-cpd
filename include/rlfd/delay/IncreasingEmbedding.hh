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
#ifndef __INCREASING_EMBEDDING_HH__
#define __INCREASING_EMBEDDING_HH__

#include <rlfd/delay/GammaTest.hh>
#include <rlfd/delay/DelayEmbedding.hh>

#include <Eigen/Core>

#include <limits>

namespace rlfd {
namespace delay {

/**
 * @param The input scalar time series
 * @param The lag parameter (found by some other method such as the average
 * displacement)
 * @param The maximum embedding dimension up to which to compute the gamma
 * statistics.
 */
Eigen::VectorXd IncreasingEmbedding(const Eigen::VectorXd& ts, int lag, int max_dimension, int nn=20)
{
  Eigen::VectorXd gamma_dimension(max_dimension);

  for (int m = 1; m <= max_dimension; m++) {
    // Input time series for the gamma test is the m-dimension embedding
    Eigen::MatrixXd ts_embedded;
    rlfd::delay::DelayEmbedding::Embed(ts, m, lag, ts_embedded);

    // Output is defined as the next point after the last component of the
    // previous embedding vector.
    int M = ts.size() - m*lag;
    Eigen::VectorXd next_points(M);
    for (int i = 0; i < M; i++) {
      next_points[i] = ts[i + m*lag];
    }

    auto slope_intercept = rlfd::delay::GammaTest(ts_embedded.topRows(M), next_points, nn);
    gamma_dimension[m-1] = std::abs(slope_intercept[1]);
  }

  return gamma_dimension;
}


} // namespace delay
} // namespace rlfd

#endif // __INCREASING_EMBEDDING_HH__
