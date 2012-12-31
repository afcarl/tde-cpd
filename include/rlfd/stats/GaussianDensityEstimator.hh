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
#include <numeric>

namespace rlfd {
namespace stats {

class GaussianDensityEstimator
{
 public:
  /**
   * Initialize a Gaussian Density estimator with autocalibration of the
   * bandwidth based on average distance to the d nearest neighbors
   * @param d The knn parameter in EstimateSigma
   */
  GaussianDensityEstimator(double sigma = 1.0, int d = 4) : d_(d), sigma_(sigma) {};

  double GetSigma() { return sigma_; }

  double GetDimensionality() { return d_; }

  static constexpr double Pi() { return std::acos(-1.0); }

  /**
   * Compute the distance matrix for overlapping windows spread appart by one
   * sample.
   */
  void DistanceMatrix(const Eigen::MatrixXd& ts, int W, Eigen::MatrixXd& distancesOut)
  {
    double foursigma2 = 4.0*std::pow(sigma_, 2.0);
    double k = -1.0/foursigma2;
    double normalization = (1.0/(std::pow(W, 2)*std::pow(foursigma2 * Pi(), ((double) d_)/2.0)));

    const double* dataPtr = ts.data();

    // Pre-compute the self-sums
    Eigen::VectorXd selfSums(ts.rows()-W);
    for (int s = 0; s < ts.rows()-W; s++) {
      double sum = 0.0;
      const double* xPtr = dataPtr + s;
      //auto xblock = ts.block(s, 0, W, d_);

      for (int w = 0; w < W; w++) {
        for (int v = 0; v < W; v++) {
          double squaredNorm = 0.0;
          for (int j = 0; j < d_; j++) {
            double norm = xPtr[w + j*W] - xPtr[v + j*W];
            squaredNorm += norm*norm;
            //std::cout << "xblock(" << w << ", " << j << ")" << xblock(w, j) << " vs " << xPtr[w + j*W] << std::endl;
          }
          //std::cout << "Squared norm" << squaredNorm << std::endl;
          //std::cout << (xblock.row(w) - xblock.row(v)).squaredNorm() << std::endl;

          sum += std::exp(k*squaredNorm);
        }
      }

      selfSums[s] = sum;
    }
    //std::cout << selfSums << std::endl;

    // Compute for each sample
    for (int s = 1; s < ts.rows()-W; s++) {
      const double* xprimePtr = dataPtr + W*(s+1);

      // Up to diagonal
      for (int t = 0; t < s; t++) {
        const double* xPtr = xprimePtr + 1;

        // Integrated Square Error computation
        double crossSum = 0.0;
        for (int w = 0; w < W; w++) {
          for (int v = 0; v < W; v++) {

            // Squared norm
            double squaredNormCross = 0.0;
            for (int j = 0; j < d_; j++) {
              double norm = (xprimePtr + w + j*W) - (xPtr + v + j*W);
              squaredNormCross += norm*norm;
            }

            crossSum += -2.0*std::exp(k*squaredNormCross);
          }
        }

        distancesOut(s, t) = normalization*(selfSums[s] + crossSum + selfSums[t]);
      }
    }

  }

  /**
   * Estimate the pdf only in a fixed-size window
   * @param X Row vectors to be estimated.
   * @param sigma Found by EstimateSigma
   * @param d The knn parameter in EstimateSigma
   * @param W The window size
   *
   */
  template<typename Derived>
double operator()(const Eigen::Block<Derived>& X, const Eigen::Block<Derived>& Xprime)
{
  int W = X.rows();
  double foursigma2 = 4.0*std::pow(sigma_, 2.0);
  double normalization = 1.0/(std::pow(W, 2)*std::pow(foursigma2 * Pi(), ((double) d_)/2.0));
  int k = -1.0/foursigma2;


  const double* ptrX = X.data();
  const double* ptrXprime = Xprime.data();

  double sum = 0.0;
  for (int w = 0; w < W; w++) {
    for (int v = 0; v < W; v++) {

      double squaredNormSelf = 0.0;
      double squaredNormCross = 0.0;
      double squaredNormPast = 0.0;
      double norm = 0.0;
      for (int j = 0; j < d_; j++) {
        norm = (ptrXprime + w + j*W) - (ptrXprime + v + j*W);
        squaredNormSelf += norm*norm;

        norm = (ptrXprime + w + j*W) - (ptrX + v + j*W);
        squaredNormCross += norm*norm;

        norm = (ptrX + w + j*W) - (ptrX + v + j*W);
        squaredNormPast += norm*norm;
      }
      sum += std::exp(k*squaredNormSelf) - 2.0*std::exp(k*squaredNormCross) + std::exp(k*squaredNormPast);

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
      avg_dist += std::sqrt(dists[i][k]);
    }
    avg_dists(i) = avg_dist/((double) (knn - 1));
  }

  // Compute the average over the whole sample
  return avg_dists.mean();
}

/**
 * Set parameters of this KDE instance using the EstimateSigma method.
 * An index is created automatically for this purpose.
 * @param sample Sample points from which to infer the parameters
 */
void Calibrate(const Eigen::MatrixXd& sample)
{
  // TODO get rid of this copying
  flann::Matrix<double> input(new double[sample.rows()*sample.cols()], sample.rows(), sample.cols());
  for (int i = 0; i  < sample.rows(); i++) {
    for (int j = 0; j < sample.cols(); j++) {
      input[i][j] = sample(i, j);
    }
  }

  flann::Index<flann::L2<double> > index(input, flann::KDTreeSingleIndexParams());
  index.buildIndex();

  sigma_ = EstimateSigma(sample, sample.cols(), index);
  d_ = sample.cols();
}

private:

int d_;
double sigma_;

};

} // namespace stats
} // namespace rlfd
#endif // __GAUSSIANDENSITYESTIMATOR_HH__

