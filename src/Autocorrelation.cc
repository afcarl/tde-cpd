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
#include <rlfd/utils/ImportExport.hh>
#include <rlfd/utils/Autocorrelation.hh>

#include <Eigen/Core>
#include <iostream>

int main(int argc, char** argv)
{
  if (argc > 2) {
    std::cerr << "Compute the sample autocorrelation function" << std::endl; 
    std::cerr << "Usage: autocorrelation [FILE]" << std::endl;
    return -1;
  }

  // Open up the .mat file. Version 7, and 7.3 are not supported and result in a
  // segmentation fault with the current version of libmatio in Ubuntu. 
  Eigen::MatrixXd ts;
  if (argc == 2) {
    rlfd::utils::Import(argv[2], ts);
  } else {
    rlfd::utils::Import(ts);
  }

  // Compute the autocorrelation coefficients 
  Eigen::VectorXd acoeffs;
  rlfd::utils::Autocorrelation(ts, acoeffs); 

  std::cout << acoeffs << std::endl;

  return 0;
}

