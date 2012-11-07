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
#ifndef __GAMMA_TEST_H__
#define __GAMMA_TEST_H__

#include <Eigen/Dense>
#include <flann/flann.hpp>

namespace rlfd {
namespace delay {

/**
 * Compute the Gamma Test on the set of points
 * @param in The input x time series. 
 * @param out The output y time series. 
 * @return A vector where the second component is the gamma statistic while the first
 * is the slope of the regression line for the pairs coordinates (gamma, delta) and is a 
 * a good indicator of the complexity	of the surface defined by f. 
 */
Eigen::VectorXd GammaTest(const Eigen::MatrixXd& in, const Eigen::VectorXd& out, int nn) 
{
  // Type conversions. No memory duplication. 
  // @fixme seems to be no way to avoid const_cast unless the data is duplicated 
  flann::Matrix<double> input(const_cast<double*>(in.data()), in.rows(), in.cols());
  flann::Index<flann::L2<double> > index(input, flann::KDTreeSingleIndexParams());
  index.buildIndex();

  // Compute the k-nearest neighbors for every input points
  flann::Matrix<int> indices(new int[input.rows*(nn+1)], input.rows, (nn+1));
  flann::Matrix<double> dists(new double[input.rows*(nn+1)], input.rows, (nn+1));
  index.knnSearch(input, indices, dists, (nn+1), flann::SearchParams(128));

  // Compute delta and gamma for a range of k
  Eigen::MatrixXd deltas(nn, 2);
  Eigen::VectorXd gammas(nn);
  for (int p = 1; p < (nn+1); p++) {
    double average_input_dist = 0.0;
    double average_output_dist = 0.0;
    for (int i = 0; i < in.rows(); i++) {
      average_input_dist += dists[i][p];
      // note y_{N[i, k]} is not necessarily the kth 
      // nearest neighbour of yi in output space
      int kthnn = indices[i][p];
      average_output_dist += std::pow(out[kthnn] - out[i], 2);
    }
    average_input_dist = average_input_dist/((double) in.rows());
    average_output_dist = average_output_dist/(2.0*in.rows());

    deltas(p-1, 0) = average_input_dist;
    deltas(p-1, 1) = 1;
    gammas[p-1] = average_output_dist;
  }
  // Compute the least square fit to the pairs deltas, gammas and find intercept. 
  // The intercept of the regression line converges 
  // in probability to var(r) as M goes to infinity.
  return deltas.colPivHouseholderQr().solve(gammas);
}

} // namespace delay
} // namespace rlfd

#endif
