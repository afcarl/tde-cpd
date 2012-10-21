#ifndef __GAMMA_TEST_H__
#define __GAMMA_TEST_H__

#include <Eigen/Core>
#include <Eigen/Dense>
#include <flann/flann.hpp>

namespace rlfd {
class GammaTest
{
public:
  /**
   * @param nn The number of nearest neighbors to consider.  
   */
  GammaTest(unsigned nn=10) : nn_(nn) {};
  virtual ~GammaTest() {};

  /**
   * Compute the Gamma Test on the set of points
   * @param points Series of points as rows of a matrix 
   * @return A vector where the second component is the gamma statistic while the first
   * is the slope of the regression line for the pairs coordinates (gamma, delta) and is a 
   * a good indicator of the complexity	of the surface defined by f. 
   */
  Eigen::Vector2d estimate(const Eigen::MatrixXd& in, const Eigen::MatrixXd& out) {
    // Type conversions. No memory duplication. 
    flann::Matrix<double> input(in.data(), in.rows(), in.cols());
    flann::Matrix<double> output(out.data(), out.rows(), out.cols());

    flann::Index<flann::L2<double> > index(input, flann::KDTreeIndexParams(4));

    // Compute the k-nearest neighbors for every input points
    flann::Matrix<int> indices(new int[query.rows*nn_], query.rows, nn_);
    flann::Matrix<double> dists(new double[query.rows*nn_], query.rows, nn_);
    index.knnSearch(query, indices, dists, nn_, flann::SearchParams(128));

    // Compute delta and gamma for a range of k
    Eigen::MatrixXd deltas(nn_, 2);
    Eigen::VectorXd gammas(nn_);
    for (int p = 0; p < nn_; p++) {
      double average_input_dist = 0;
      double average_output_dist = 0;
      for (int i = 0; i < in.rows(); i++) {
        int kthnn = indices[i][p-1];
        average_input_dist += (in.row(kthnn) - in.row(i)).array().pow(2);

        // note y_{N[i, k]} is not necessarily the kth 
        // nearest neighbour of yi in output space
        average_output_dist += (out.row(kthnn) - out.row(i)).array().pow(2);
      }
      average_input_dist = average_input_dist/in.rows();
      average_output_dist = average_output_dist/out.rows();

      deltas(p, 0) = average_input_dist;
      deltas(p, 1) = 1;
      gammas[p] = average_output_dist;
    }

    // Compute the least square fit to the pairs deltas, gammas and find intercept. 
    // The intercept of the regression line converges 
    // in probability to var(r) as M goes to infinity.
    return deltas.fullPivHouseholderQr().solve(gammas);
  }

private:
  unsigned nn_;
};
}

#endif