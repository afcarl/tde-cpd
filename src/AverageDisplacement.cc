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
#include <rlfd/utils/Gnuplot.hh>
#include <rlfd/utils/ImportExport.hh>
#include <rlfd/delay/UnbiasedMultipleAutocorrelation.hh>

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

  // Import the matrix
  Eigen::MatrixXd ts;
  rlfd::utils::Import(filename, ts); 

  // Compute the S_m^2 statistics for a range of tau values
  Eigen::VectorXd ads = rlfd::delay::UnbiasedMultipleAutocorrelation(ts, embedding_dimension); 

  std::cout << ads << std::endl;
  rlfd::utils::Gnuplot gnuplot;
  gnuplot(ads);

  return 0;
}
