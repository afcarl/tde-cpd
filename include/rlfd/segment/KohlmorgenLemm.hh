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
#ifndef __KOHLMORGEN_LEMM_HH__
#define __KOHLMORGEN_LEMM_HH__

#include <rlfd/Model.hh>
#include <rlfd/delay/DelayEmbedding.hh>

#include <Eigen/Core>
#include <vector>
#include <memory>
#include <numeric>

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
class KohlmorgenLemm
{
 public:
  KohlmorgenLemm(DistanceFunctorType& distanceFunctor, int W=50, double C=0.0) :
      distanceFunctor(&distanceFunctor), W(W), regularizer(C)
  {
    cpaths.resize(1, 0.0);
    opaths.resize(1, 0.0);
    distances.resize(1, 0.0);
  }

  virtual ~KohlmorgenLemm() {};

 protected:
  std::vector<double> cpaths;
  std::vector<double> opaths;
  std::vector<double> distances;

  DistanceFunctorType* distanceFunctor;

  int W;
  double regularizer;

 public:
  /**
   * Regularization constant C that subsumes varsigma and k
   */
  double GetRegularizer() { return regularizer; }

  void AddObservation(const Eigen::MatrixXd& embTs, int T)
  {
    auto latest_window = embTs.block(T-(W-1), 0, W, embTs.cols());

    // C_T(T-1)
    double cpath_latest = 0.0;
    for (unsigned t = 0; t < cpaths.size(); t++) {
      if (t == 0) {
        cpath_latest = 0.0;
      } else {
        cpath_latest = std::min(cpath_latest, opaths[t-1] + GetRegularizer());
      }

      auto window_t = embTs.block(t, 0, W, embTs.cols());
      distances[t] = (*distanceFunctor)(latest_window, window_t);
      cpath_latest += distances[t];

      if (cpath_latest < opaths[t]) {
        opaths[t] = cpath_latest;
        std::cerr << "Updating opath" << std::endl;
      }
    }
    std::cerr << "C_T(T-1) " << cpath_latest << std::endl;
    cpaths.push_back(cpath_latest);
    distances.push_back(0.0);

    // C_T(T)
    for (unsigned t = 0; t < cpaths.size(); t++) {
      cpaths[t] = distances[t] + std::min(cpaths[t], opaths.back() + GetRegularizer());
    }
    std::cerr << "C_T(T) updated" << std::endl;

    // Choose the state with the minimum cost
    opaths.push_back((*std::min_element(cpaths.begin(), cpaths.end())));
  }

};

} // namespace segmentation
} // namespace rlfd

#endif
