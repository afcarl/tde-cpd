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
#ifndef __GAUSSIANDENSITYESTIMATOR_HH__
#define __GAUSSIANDENSITYESTIMATOR_HH__

#include <Eigen/Core>
#include <flann/flann.hpp>

namespace rlfd {
namespace stats {

class GaussianDensityEstimator
{
  constexpr double pi(void) { return std::acos(-1.0); }

  /**
   * Estimate the pdf only in a fixed-size window
   * @param X Row vectors to be estimated.
   * @param sigma Found by EstimateSigma
   * @param d The knn parameter in EstimateSigma
   * @param W The window size
   *
   */
  static double DistanceWindowed(const Eigen::MatrixXd& X, int sigma, int d, int W=50)
  {
    double 4sigma2 = 4.0*std::pow(sigma, 2);
    double normalization = 1.0/(std::pow(W, 2)*std::pow(4sigma2, d/2.0));

    double sum = 0.0;
    for (int w = 0; w < W-1; w++) {
      for (int v = 0; v < W-1; v++) {
        sum += std::exp(-1.0 * (X.row(tbar - w) - W.row(tbar - v)).squaredNorm()/4sigma2);
        sum += std::exp(-1.0 * (X.row(tbar - w) - X.row(t - v)).squaredNorm()/4sigma2)*-2.0;
        sum += std::exp(-1.0 * (X.row(t - w) - X.row(t - v)).squaredNorm()/4sigma2);
      }
    }

    return normalization*sum;
  }

  /**
   * Estimate the sigma parameter by taking the mean distance of the data points
   * to their k nearest neighbors.
   * @param X Row vectors to be estimated.
   * @param knn The number of nearest neighbors over which to take average
   * distance
   * @return The average distance to the knn in the sample X
   */
  static double EstimateSigma(const Eigen::MatrixXd& X, int knn, flann::Index<flann::L2<double>>& index)
  {
    // Compute the knn for all of the data points
    knn += 1;
    flann::Matrix<int> indices(new int[X.rows()*knn], X.rows(), knn);
    flann::Matrix<double> dists(new double[X.rows()*knn], X.rows(), knn);
    flann::Matrix<double> query(new double[X.rows()*X.cols()], X.rows(), X.cols());
    for (int i = 0; i < X.rows(); i++) {
      for (int j = 0; j < X.cols(); j++) {
        query[i][j] = X(i, j);
      }
    }
    index.knnSearch(query, indices, dists, knn, flann::SearchParams(128));

    // Compute the average distance to the knn of each point
    Eigen::VectorXd avg_dists(X.rows());
    for (int i = 0; i < X.rows(); i++) {
      double avg_dist = 0.0;
      for (int k = 1; k < knn; k++) {
        avg_dist += dists[i][k];
      }
      avg_dists(i) = avg_dist/((double) (knn - 1));
    }

    // Compute the average over the whole sample
    return avg_dists.mean();
  }

};

} // namespace stats
} // namespace rlfd
#endif // __GAUSSIANDENSITYESTIMATOR_HH__

