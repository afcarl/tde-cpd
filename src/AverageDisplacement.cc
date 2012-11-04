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
#include <rlfd/UnbiasedMultipleAutocorrelation.hh>
#include <rlfd/utils/Matio.hh>
#include <rlfd/utils/Gnuplot.hh>

#include <string>
#include <iostream>

int main(int argc, char** argv)
{
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " [FILE] [Embedding dimension] [OPTION]" << std::endl;
    std::cerr << "  -p --plot    Plot S_m^2 as a function of tau" << std::endl;
    return -1;
  }
  char* filename = argv[1];
  int embedding_dimension = std::stoi(argv[2]);

  // Open up the .mat file. Version 7, and 7.3 are not supported and result in a
  // segmentation fault with the current version of libmatio in Ubuntu. 
  rlfd::utils::Matio matio;
  matio.Open(filename);
  
  Eigen::MatrixXd ts;
  matio.Read("y", ts);
  matio.Close();

  // Compute the S_m^2 statistics for a range of tau values
  Eigen::VectorXd ads = rlfd::UnbiasedMultipleAutocorrelation(ts, embedding_dimension); 

  std::cout << ads << std::endl;
  rlfd::utils::Gnuplot gnuplot;
  gnuplot(ads);

  return 0;
}
