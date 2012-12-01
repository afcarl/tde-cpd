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
#include <deque>
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
  KohlmorgenLemm(DistanceFunctorType& distanceFunctor, unsigned W=50, double varsigma=0, double k=0) :
      distanceFunctor(&distanceFunctor), W(W), varsigma(varsigma), k(k) {};

  virtual ~KohlmorgenLemm() {};

 protected:
  /**
   * Hidden state of the HMM that we are trying to infer
   */
  struct State {
    State(double cpath, int index, int prototypeIndex):
        cpath(cpath), index(index), prototypeIndex(prototypeIndex) {}

    // Cost of the c-path finishing in this state
    double cpath;

    int index;

    int prototypeIndex;
  };
  std::deque<State> states;

  // Maintain a history of the optimal paths costs
  std::deque<double> opaths;

  DistanceFunctorType* distanceFunctor;

  double W;
  // Regularization constants
  double varsigma;
  double k;

 public:
  /**
   * Regularization constant C that subsumes varsigma and k
   */
  constexpr double GetRegularizer() { return 2*std::pow(varsigma, 2)*std::log(k); }

  /**
   * Add an observation (delay-embedded).
   * When the number of observations is sufficient
   * to build a model, the segmentation will get
   * updated automatically
   * @param x An observation vector
   */
  void AddState(const Eigen::MatrixXd& embTs, int T)
  {
    auto last_window = embTs.block(T-W, 0, W, embTs.cols());

    // Add a new state to the HMM for this observation
    auto distance = (*distanceFunctor)(last_window,
                                    embTs.block((T-1)-W, 0, W, embTs.cols()));
    states.push_front(State(distance, T, T));
    State& latest_state = states.front();

    // Compute the cpath at T-1 for the new state (we don't have it already)
    for (int t = W+1; t < T-1; t++) {
      // Compute the distance of the new window to the window at time t
      auto distance = (*distanceFunctor)(last_window,
                                      embTs.block(t-W, 0, W, embTs.cols()));


      latest_state.cpath = distance + std::min(latest_state.cpath, opaths[t-1] + GetRegularizer());

      if (latest_state.cpath < opaths[t]) {
        opaths[t] = latest_state.cpath;
      }
    }

    // Recomputing the cpath for each state finishing at time T
    for (unsigned s = 0; s < states.size(); s++) {
      // Get the window corresponding to this state
      auto swindow = embTs.block(states[s].index-W, 0, W, embTs.cols());
      auto distance = (*distanceFunctor)(swindow, last_window);
      states[s].cpath = distance + std::min(states[s].cpath, opaths.front() + GetRegularizer());
    }

    // Find the mininum cost constrained path
    auto mincost_state = std::min_element(states.begin(), states.end(), [](const State& a, const State& b) { return a.cpath < b.cpath; });
    opaths.push_front(mincost_state->cpath);
  }
};

} // namespace segmentation
} // namespace rlfd

#endif
