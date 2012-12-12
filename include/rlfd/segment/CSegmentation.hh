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
#ifndef __CSEGMENTATION_HH__
#define __CSEGMENTATION_HH__

#include <rlfd/Model.hh>
#include <rlfd/delay/DelayEmbedding.hh>

#include <Eigen/Core>

namespace rlfd {
namespace segment {

/**
 * A class to perform time series segmentation.
 * It takes as a argument a Model type
 *
 * Kohlmorgen, J., & Lemm, S. (2001). A dynamic HMM for on-line segmenation of
 * sequation data. Advances in Neural Information Processing Systems 14 (NIPS
 * 2001) (pp. 793-800). Vancouver, British Columbia, Canada: MIT Press.
 */
template <class DistanceFunctorType>
class CSegmentation
{
 public:
  CSegmentation(DistanceFunctorType& distanceFunctor, int W=50, double C=0.0) :
      distanceFunctor(&distanceFunctor), W(W), C(C) {};
  virtual ~CSegmentation() {};

  template<typename Derived>
  void operator()(const Eigen::MatrixBase<Derived>& ts)
  {
    unsigned T = ts.rows();

    // Initialize distance  matrix
    Eigen::MatrixXd distances(T-W, T-W);
    distances.setZero();
    std::cout << "Computing distances" << std::endl;

    distanceFunctor->DistanceMatrix(ts, W, distances);

    std::cout << "Done computing distances" << std::endl;

    // Init t=W
    Eigen::MatrixXd opaths(T-W, T-W);
    opaths.col(0) = distances.col(0);
    std::cout << "Initialized t=W" << std::endl;

    // t = W+1
    for (int t = 1; t < opaths.cols(); t++) {
      double h = opaths.col(t-1).minCoeff() + C;
      for (int s = 0; s < opaths.rows(); s++) {
        opaths(s, t) = (t > s ? distances(t, s) : distances(s, t)) + std::min(opaths(s, t-1), h);
      }
    }

    // Termination at t = T
    //double ofinal = opaths.col(opaths.cols()-1).minCoeff();
  }

 protected:
  DistanceFunctorType* distanceFunctor;

  int W;
  double C;

};

} // namespace segmentation
} // namespace rlfd

#endif
