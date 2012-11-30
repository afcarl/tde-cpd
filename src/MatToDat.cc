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
#include <rlfd/utils/Matio.hh>
#include <Eigen/Core>
#include <iostream>
#include <limits>

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Convert a Matlab's .mat file to a tabular raw .dat file" << std::endl;
    std::cerr << "Usage: mattodat [FILE]" << std::endl;
    return -1;
  }
  // Import 
  Eigen::MatrixXd out;
  rlfd::utils::Matio<Eigen::MatrixXd> mat;
  mat.Open(argv[1]);
  mat.Read(out);
  mat.Close();

  std::cout.precision(std::numeric_limits<double>::digits10);
  std::cout << out << std::endl;

  return 0;
}
