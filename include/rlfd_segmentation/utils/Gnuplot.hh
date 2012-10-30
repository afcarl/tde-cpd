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
#ifndef __GNUPLOT_HH__
#define __GNUPLOT_HH__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <stdexcept>
#include <Eigen/Core>

namespace rlfd {
namespace utils {

class Gnuplot 
{
 public:
  Gnuplot()
  {
    gp_ = popen("gnuplot -persist","w"); 
    if (gp_ == NULL) {
      throw std::runtime_error(strerror(errno));
    }
  }

  ~Gnuplot() 
  {
    fflush(gp_);
    fclose(gp_);
  }

  void operator()(const Eigen::VectorXd& ind, const Eigen::VectorXd& dep)
  {
    // TODO Check if dim is the same
    fprintf(gp_, "plot '-' with points\n");
    for (int i = 0; i < ind.size(); i++) {
      fprintf(gp_, "%.15le %.15le\n", ind[i], dep[i]); 
      fflush(gp_);
    }
    fprintf(gp_, "\ne\n");
  }

  void operator()(const std::vector<double>& ind, const std::vector<double>& dep)
  {
    this->operator()(Eigen::VectorXd::Map(&ind[0], ind.size()), Eigen::VectorXd::Map(&dep[0], dep.size()));
  }

 private:
  FILE* gp_;
};

} // namespace utils
} // namespace rlfd
#endif 
