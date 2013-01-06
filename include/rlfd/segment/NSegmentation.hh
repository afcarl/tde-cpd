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
 * Implements the N-Segmentation algorithm from:
 *
 * J. Kohlmorgen, "On Optimal Segmentation of Sequential Data", in
 * Proceedings of the 13th International IEEE workshop on Neural Networks for
 * Signal Processing, 2003, pp. 449–458.
 *
 * @param distances The pre-computed distance matrix for the vectors of ts
 * @param C The regularization constant
 */
template<typename Derived>
void NSegmentation(const Eigen::MatrixBase<Derived>& distances, unsigned N)
{

  unsigned T = distances.cols();

  // Maunsignedain the costs for n-segments segmentations
  std::vector<Eigen::MatrixXd> costs;
  costs.resize(N);

  // Initialization at t = 0 
  costs[0].resize(T, T);
  costs[0].setZero();
  costs[0].col(0) = distances.col(0);
//  std::cout << costs[0].col(0) << std::endl; 

  for (unsigned i = 1; i < N; i++) {
    costs[i].resize(T, T);
    costs[i].setZero();
    costs[i].col(0) = Eigen::VectorXd::Constant(T, std::numeric_limits<double>::infinity());
  }  

  // Recusion t = 1 .. T
  Eigen::MatrixXd::Index minIndex;
  double minUnconstrained = 0.0;

  for (unsigned t = 1; t < T; t++) {
    for (unsigned n = 0; n < N; n++) {
      for (unsigned s = 0; s < T; s++) {
        double distance = t > s ? distances(t, s) : distances(s, t);
        if (n != 0) {
          if ((unsigned) minIndex != s) {
            (costs[n])(s, t) = distance + std::min((costs[n])(s, t - 1), minUnconstrained); 
          } else {

            auto segmentA = costs[n-1].col(t-1).segment(0, s);
            double minA = std::numeric_limits<double>::infinity(); 
            if (segmentA.size() != 0) {
              minA = segmentA.minCoeff();
            }

            auto segmentB = costs[n-1].col(t-1).segment(s+1, T-(s+1));
            double minB = std::numeric_limits<double>::infinity(); 
            if (segmentB.size() != 0) {
              minB = segmentB.minCoeff();
            }

            (costs[n])(s, t) = distance + std::min((costs[n])(s, t - 1), std::min(minA, minB)); 
          }
        } else {
          (costs[n])(s, t) = distance + (costs[n])(s, t - 1);
        }
      } // for all s
      minUnconstrained = costs[n].col(t-1).minCoeff(&minIndex); 
    } 
  } 

  // Termination 
  for (unsigned n = 0; n < N; n++) {
    std::cout << n+1 << "    " << costs[n].col(T-1).minCoeff(&minIndex) << std::endl;
  }
}


} // namespace segmentation
} // namespace rlfd

#endif
