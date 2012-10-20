#ifndef __GAMMA_TEST_H__
#define __GAMMA_TEST_H__

#include <Eigen/Core>
#include <flann/flann.hpp>

namespace rlfd {
class GammaTest
{
public:
  /**
   * @param nn The number of nearest neighbors to consider.  
   */
  GammaTest(unsigned nn=10) : nn_(nn);
  ~GammaTest();

  /**
   * Compute the Gamma Test on the set of points
   * @param points Series of points as rows of a matrix 
   */
  double estimate(const Eigen::MatrixXd& input, const Eigen::MatrixXd& output) {
    flann::Index<flann::L2<double> > index(input, flann::KDTreeIndexParams(4));

    // Compute the k-nearest neighbors for every input points
    flann::Matrix<int> indices(new int[query.rows*nn_], query.rows, nn_);
    flann::Matrix<double> dists(new double[query.rows*nn_], query.rows, nn_);
    index.knnSearch(query, indices, dists, nn_, flann::SearchParams(128));

    // Compute delta and gamma for a range of k
    Eigen::Matrix<double, nn_, 2> deltas;
    Eigen::Matrix<double, nn_, 1> gammas;
    for (int p = 0; p < nn_; p++) {
      double average_input_dist = 0;
      double average_output_dist = 0;
      for (int i = 0; i < input.rows(); i++) {
        int kthnn = indices[i][p-1];
        average_input_dist += std::pow(input.row(kthnn) - input.row(i), 2);

        // note y_{N[i, k]} is not necessarily the kth 
        // nearest neighbour of yi in output space
        average_output_dist += std::pow(output.row(kthnn) - output.row(i), 2);
      }
      average_input_dist = average_input_dist/input.rows();
      average_output_dist = average_output_dist/output.rows();

      deltas(p, 0) = average_input_dist;
      deltas(p, 1) = 1;
      gammas[p] = average_output_dist;
    }

    // Compute the least square fit to the pairs deltas, gammas and find intercept. 
    Eigen::Vector2d x = deltas.fullPivHouseholderQR().solve(gammas);

    // The intercept of the regression line converges 
    // in probability to var(r) as M goes to infinity.

    return x[1];
  }

private:
  unsigned nn_;


};
}

#endif
