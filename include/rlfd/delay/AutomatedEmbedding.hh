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
#ifndef __AUTOMATED_EMBEDDING_HH__
#define __AUTOMATED_EMBEDDING_HH__

#include <rlfd/delay/AverageDisplacement.hh>
#include <rlfd/delay/DelayEmbedding.hh>
#include <rlfd/delay/GammaTest.hh>

namespace rlfd {
namespace delay {

Eigen::VectorXd AutomatedEmbedding(const Eigen::VectorXd& ts, int max_lag=50, int max_dimension=10, int nn=20)
{
  Eigen::VectorXd gamma_dimension(max_dimension);

  Eigen::VectorXd previous_statistics(2); 
  previous_statistics[0] = 0;
  previous_statistics[1] = 1;

  int previous_th = 1;
  for (int m = 1; m <= max_dimension; m++) {
    // Estimate the time lag for m by the average displacement method
    Eigen::VectorXd ads = rlfd::delay::AverageDisplacement(ts, m, max_lag); 
    double initial = (ads[2] - ads[0])/2.0;
    int lag;
    for (lag = 2; lag < ads.size() - 1; lag++) {
      if ((ads[lag+1] - ads[lag-1])/2.0 <= 0.4*initial) {
        break;
      }
    }

    // Estimate the maximum error that could be achieved by fitting the best smooth
    // non-linear model of this dimension and the lag found above. 
    Eigen::MatrixXd ts_embedded;
    rlfd::delay::DelayEmbedding::Embed(ts, m, lag, ts_embedded);

    // Create the output vector for the gamma statistics
    int M = ts.size() - m*lag;
    Eigen::VectorXd next_points(M);
    for (int i = 0; i < M; i++) {
      next_points[i] = ts[i + m*lag];
    }
    auto statistics = rlfd::delay::GammaTest(ts_embedded.topRows(M), next_points, nn);
    std::cout << "statistics m " << m << " " << lag << " " << statistics << std::endl;

    // Stop if local minimum is reached
    if (statistics[0] > 0.0 && statistics[1] < 0.20 && std::abs(previous_statistics[1]) < std::abs(statistics[1])) {
      Eigen::VectorXd parameters(2);
      parameters[0] = m-1;
      parameters[1] = previous_th;
      return parameters;
    }  
    previous_statistics = statistics;
    previous_th = lag;
  }

  // Should not be reached if proper parameters are found
  Eigen::VectorXd parameters(2);
  parameters[0] = 1;
  parameters[1] = 1;
  return parameters;
}

} // namespace delay
} // namespace rlfd

#endif // __AUTOMATED_EMBEDDING_HH__
