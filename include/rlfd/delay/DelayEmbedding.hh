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
#ifndef __DELAY_EMBEDDING_HH__
#define __DELAY_EMBEDDING_HH__

#include <rlfd/Model.hh>
#include <Eigen/Core>

namespace rlfd {
namespace delay {

class DelayEmbedding : Model {
 public:
  double distance(const Model& other) const override { return 0; };

  /**
   * Embbed a sclar time series
   * @param ts The measured time series
   * @param m The embedding dimension
   * @param lag The lag parameter
   * @param out M x m matrix where M = N-(m-1)*lag
   */
  static void Embed(const Eigen::VectorXd& ts, int m, int lag, Eigen::MatrixXd& out)
  {
    int M = ts.size() - (m-1)*lag;
    out.resize(M, m);

    for (int i = 0; i < M; i++) {
      for (int j = 0; j <= (m-1); j++) {
        out(i, j) = ts[i + j*lag];
      }
    }
  }
};

} // namespace delay
} // namespace rlfd
#endif // __DELAY_EMBEDDING_HH__
