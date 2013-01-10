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
#ifndef __LOWERINTERSECTION_HH__
#define __LOWERINTERSECTION_HH__

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
void LowerIntersection(const Eigen::MatrixBase<Derived>& costs)
{
  std::vector<double> slope(100, 0.0);
  std::vector<double> intersect(100, 0.0);

  const unsigned N = costs.rows();

  unsigned k = 0;
  slope[0] = 1.0;

  for (unsigned n = 2; n < N; n++) {
    k = k + 1;

    intersect[k] = (costs[slope[k-1]-1] - costs[n-1])/(n - slope[k-1]);

    while (k > 1 && intersect[k] > intersect[k-1]) {
      k = k - 1;

      intersect[k] = (costs[slope[k-1]-1] - costs[n-1])/(n - slope[k-1]);
    }

    slope[k] = n;
  }

  for (int i = 0; i < 50; i++) {
  std::cout << "C_k " << intersect[i] << std::endl;
  }
}

} // namespace segment
} // namespace rlfd
#endif // __LOWERINTERSECTION_HH__
