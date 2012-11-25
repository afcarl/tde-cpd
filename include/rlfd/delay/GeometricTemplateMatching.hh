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
#ifndef __GEOMETRICTEMPLATEMATCHING_HH__
#define __GEOMETRICTEMPLATEMATCHING_HH__

#include <flann/flann.hpp>

namespace rlfd {
namespace delay {

/**
 * Geometric Template Matching algorithm (GeTM).
 *
 * Described in:
 * Activity and Gait Recognition with Time-Delay Embeddings (AAAI 2010),
 * with S. Mannor and D. Precup, July 2010.
 *
 * @param index A pre-built index for the model
 * @param testEmb The embedded test time series
 * @param scores Output vector into which the scores are written.
 */
void GeometricTemplateMatching(DelayEmbedding& model, const Eigen::MatrixXd& testEmb, Eigen::VectorXd& outscores, int seglength=32, int nn=4)
{
  int M = testEmb.rows();

  // Score at each i
  Eigen::VectorXd r(M - seglength);

  // Pre-compute the nearest neighbors in the base model for each vector
  // of the test sequence.
  flann::Matrix<int> indices(new int[testEmb.size()*nn], testEmb.size(), nn);
  flann::Matrix<double> dists(new double[testEmb.size()*nn], testEmb.size(), nn);

  // Would need to have testEmb in row-major in order to avoid copying
  flann::Matrix<double> query(new double[testEmb.rows()*testEmb.cols()], testEmb.rows(), testEmb.cols());
  for (int i = 0; i < testEmb.rows(); i++) {
    for (int j = 0; j < testEmb.cols(); j++) {
      query[i][j] = testEmb(i, j);
    }
  }
  model.GetIndex().knnSearch(query, indices, dists, nn, flann::SearchParams(128));
  auto modelMat = model.GetMatrix();

  for (int i = 0; i < (M - seglength); i++) {
    for (int j = i; j < (i + seglength - 1); j++) {
      // Collect the k neareast neighbors at v_j in the base model
      Eigen::MatrixXd U(nn, modelMat.cols());
      Eigen::MatrixXd Usuccessor(nn, modelMat.cols());

      int nn_found = 0;
      for (int k = 0; k < nn; k++) {
        int nn_idx = indices[j][k];
        if (nn_idx == modelMat.rows()-1) {
          continue;
        }
        U.row(nn_found) = modelMat.row(nn_idx);
        Usuccessor.row(nn_found) = modelMat.row(nn_idx+1);
        nn_found += 1;
      }
      U.resize(nn_found, modelMat.cols());
      Usuccessor.resize(nn_found, modelMat.cols());

      // Compute the mean vector from v_j to v_{j+1}
      Eigen::VectorXd nnAvg = U.colwise().mean();
      Eigen::VectorXd nnSuccessorAvg = Usuccessor.colwise().mean();

      // Bring the vectors to the origin
      Eigen::VectorXd u = (nnSuccessorAvg - nnAvg);
      Eigen::VectorXd v = testEmb.row(j+1) - testEmb.row(j);

      // Compute the similarity score
      // TODO Don't we know the score up to the last element ? r[i-1]
      double normalization = std::max(u.squaredNorm(), v.squaredNorm());
      if (normalization > 0.0) {
        r[i] = r[i] + u.dot(v)/normalization;
      }
    }
  }

  outscores = r;
}

} // namespace delay
} // namespace rlfd
#endif // __GEOMETRICTEMPLATEMATCHING_HH__

