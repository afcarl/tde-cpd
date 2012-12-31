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
 * Implements the C-Segmentation algorithm from:
 *
 * J. Kohlmorgen, "On Optimal Segmentation of Sequential Data", in
 * Proceedings of the 13th International IEEE workshop on Neural Networks for
 * Signal Processing, 2003, pp. 449–458.
 *
 * @param distances The pre-computed distance matrix for the vectors of ts
 * @param C The regularization constant
 */
template<typename Derived>
void CSegmentation(const Eigen::MatrixBase<Derived>& distances, double C)
{
  unsigned T = distances.cols();

  // Init t = 1
  Eigen::MatrixXd opaths(T, T);
  opaths.col(0) = distances.col(0);

  // t = 2..T
  for (int t = 1; t < opaths.cols(); t++) {
    double h = opaths.col(t-1).minCoeff() + C;
    for (int s = 0; s < opaths.rows(); s++) {
      opaths(s, t) = (t > s ? distances(t, s) : distances(s, t)) + std::min(opaths(s, t-1), h);
    }
  }

  // Termination at t = T
  for (int t = 0; t < opaths.cols(); t++) {
    int i;
    opaths.col(t).minCoeff(&i);
    std::cout << i << std::endl;
  }

}


} // namespace segmentation
} // namespace rlfd

#endif
