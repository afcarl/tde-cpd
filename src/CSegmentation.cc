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
#include <rlfd/segment/CSegmentation.hh>
#include <rlfd/stats/GaussianDensityEstimator.hh>

#include <limits>
#include <iostream>

#include <getopt.h>
#include <Eigen/Core>

void print_usage(void)
{
  std::cout << "Execute the C-Segmentation algorithm on the data passed through STDIN" << std::endl;
  std::cout << "Usage: C-Segmentation [OPTION]" << std::endl;
}

int main(int argc, char** argv)
{
  std::cout.precision(std::numeric_limits<double>::digits10);

  double regularizer = 0;
  std::string distance_file;

  // Parse arguments
  static struct option long_options[] =
  {
    {"distance-matrix", required_argument, 0, 'D'},
    {"regularizer", required_argument, 0, 'C'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "C:D:", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 'C' :
        regularizer = std::stod(optarg);
        break;
      case 'D':
        distance_file = std::string(optarg);
        break;
      default:
        print_usage();
        return -1;
    }
  }

  // Read the embedded input vectorsg
  Eigen::MatrixXd ts;
  if (optind < argc) {
    rlfd::utils::Import(argv[optind], ts);
  } else {
    rlfd::utils::Import(ts);
  }

  // Read the distance matrix
  Eigen::MatrixXd dists;
  if (distance_file != "") {
    rlfd::utils::Import(distance_file, dists);g
  }

  // Compute the segmentation
  rlfd::segment::CSegmentation(dists, regularizer);

  return 0;
}
