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

#include <flann/flann.hpp>
#include <rlfd/utils/ImportExport.hh>

#include <memory>
#include <Eigen/Core>

namespace rlfd {
namespace delay {

class DelayEmbedding
{
 public:
  // Row-major is important for compability with flann
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> EigenMatrixXdRowMajor;

  /**
   * Load the Kd-Tree index from a file
   * @param filename The path to the index file
   * @precondition The current embedded time series must be consistent with the
   * saved index.
   */
  void LoadIndex(const std::string& filename)
  {
    data = std::unique_ptr<flann::Matrix<double>>(new flann::Matrix<double>
                                                  (const_cast<double*>(embeddedTs.data()),
                                                  embeddedTs.rows(), embeddedTs.cols()));
    index = std::unique_ptr<flann::Index<flann::L2<double>>>(new flann::Index<flann::L2<double>>
                                                             (*data, flann::SavedIndexParams(filename)));
  }

  /**
   * Copy the embedded time series into this.
   * Row or column-major conversions are handled by Eigen3
   */
  template<typename EigenMatrixType>
  void SetMatrix(const EigenMatrixType& mat)
  {
    embeddedTs = mat;
  }

  /**
   * Instanciate a new kd-tree index on the points contained in the internal
   * matrix initialized by SetMatrix.
   */
  void BuildIndex()
  {

    data = std::unique_ptr<flann::Matrix<double>>(new flann::Matrix<double>
                                                  (const_cast<double*>(embeddedTs.data()),
                                                  embeddedTs.rows(), embeddedTs.cols()));

    index = std::unique_ptr<flann::Index<flann::L2<double>>>(new flann::Index<flann::L2<double>>
                                                             (*data, flann::KDTreeSingleIndexParams()));
    index->buildIndex();
  }

  /**
   * @return A non-const reference to the kd-tree index
   */
  flann::Index<flann::L2<double>>& GetIndex(void)
  {
    return (*index);
  }

  /**
   * @return a const reference to the row-major matrix used to store the points
   */
  const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>& GetMatrix(void) const
  {
    return embeddedTs;
  }

  /**
   * Embbed a sclar time series
   * @param ts The measured time series
   * @param m The embedding dimension
   * @param lag The lag parameter
   * @param out M x m matrix where M = N-(m-1)*lag
   */
  template<typename EigenMatrixType=Eigen::MatrixXd>
  static void Embed(const Eigen::VectorXd& ts, int m, int lag, EigenMatrixType& out)
  {
    int M = ts.size() - (m-1)*lag;
    out.resize(M, m);

    for (int i = 0; i < M; i++) {
      for (int j = 0; j <= (m-1); j++) {
        out(i, j) = ts[i + j*lag];
      }
    }
  }

 protected:
  EigenMatrixXdRowMajor embeddedTs;

  // Maintain the embedded points in a KD-Tree for fast retrieval
  std::unique_ptr<flann::Matrix<double>> data;
  std::unique_ptr<flann::Index<flann::L2<double>>> index;

};

} // namespace delay
} // namespace rlfd
#endif // __DELAY_EMBEDDING_HH__
