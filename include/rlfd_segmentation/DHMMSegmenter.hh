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
#ifndef __DHMMSEGMENTER_H__
#define __DHMMSEGMENTER_H__

#include <rlfd_segmentation/Model.hh>

#include <Eigen/Core>
#include <deque>
#include <memory>
#include <numeric>

namespace rlfd {
/**
 * A class to perform time series segmentation.
 * It takes as a argument a Model type
 *
 * Implements a variant of:
 *
 * Kohlmorgen, J., & Lemm, S. (2001). A dynamic hmm
 * for on-line segmentation of sequential data. Advances in Neural Information Processing Systems
 * 14 (NIPS 2001) (pp. 793–800). Vancouver, British
 * Columbia, Canada: MIT Press.
 */
template <class WindowModel>
class DHMMSegmenter
{
 public:
  DHMMSegmenter(unsigned W=50, double varsigma=0, double k=0);
  virtual ~DHMMSegmenter() {};

 private:
  struct State {
    State(double cpath, Model* model, Model* prototype): 
        cpath(cpath), model(model), prototype(prototype) {}

    // Cost of the c-path finishing at this state
    double cpath;

    // Model computed for this state. 
    // Can be a PDF or delay embedding 
    std::shared_ptr<Model> model;

    // The prototype assigned to this state
    std::shared_ptr<Model> prototype;
  };
  std::deque<State> states;

  // Maintain a history of the optimal paths costs
  std::deque<double> opaths;

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
  void AddObservation(const Eigen::VectorXd& x)
  {
    // Compute a new model 
    Model* latest_model = new WindowModel(); 
    states.push_front(State(latest_model->distance(*(states[0].model)), latest_model, latest_model));

    // t=0 to t-1
    for (unsigned t = 1; t < states.size()-1; t++) {
      // Cost for a constrained path finishing with the new model r
      // starting at 0 and finishing at time step t 
      states.front().cpath = latest_model->distance(*(states[t].model)) + std::min(states.front().cpath, opaths[t-1] + GetRegularizer()); 
      if (states.front().cpath < opaths[t]) {
        opaths[t] = states.front().cpath;
        states[t].prototype = std::shared_ptr<Model>(latest_model); // Not sure if that makes sense
      } 
    }

    // Compute the minimum cost for each state
    for (unsigned s = 0; s < states.size(); s++) {
      states[s].cpath = latest_model->distance(*(states[s].model)) + std::min(states[s].cpath, opaths.front() + GetRegularizer());
    }

    // Find the mininum cost constrained path
    auto mincost_state = std::min_element(states.begin(), states.end(), [](const State& a, const State& b) { return a.cpath < b.cpath; });
    opaths.push_front(mincost_state->cpath);
    states.front().prototype = mincost_state->model; 
  }
};

} // namespace rlfd

#endif
