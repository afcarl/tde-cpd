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
#include <rlfd/stats/GaussianDensityEstimator.hh>

#include <limits>
#include <iostream>

#include <getopt.h>
#include <Eigen/Core>

void print_usage(void)
{
  std::cout << "Compute the full distance matrix between all pair of points." << std::endl;
  std::cout << "Usage: gaussiankde [OPTION]" << std::endl;
}

int main(int argc, char** argv)
{
  std::cout.precision(std::numeric_limits<double>::digits10);

  int W = 50;
  double sigma = 1.0;
  int calibrate_flag = 0;

  // Parse arguments
  static struct option long_options[] =
  {
    {"calibrate", no_argument, &calibrate_flag, 1},
    {"window", required_argument, 0, 'w'},
    {"sigma", required_argument, 0, 's'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "w:s:", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 0:
        break;
      case 'w':
        W = std::stoi(optarg);
        break;
      case 's':
        sigma = std::stod(optarg);
        break;
      case '?':
        break;
      default:
        print_usage();
        return -1;
    }
  }

  // Read the input vectors
  Eigen::MatrixXd ts;
  if (optind < argc) {
    rlfd::utils::Import(argv[optind], ts);
  } else {
    rlfd::utils::Import(ts);
  }

  // Calibrate
  if (calibrate_flag) {
    rlfd::stats::GaussianDensityEstimator kde;
    kde.Calibrate(ts);
    std::cout << "sigma : " << kde.GetSigma() << std::endl;
    std::cout << "d: " << kde.GetDimensionality() << std::endl;
    std::cout << "w: " << W << std::endl;
    return 0;
  }

  // Default behavior: compute distance matrix
  rlfd::stats::GaussianDensityEstimator kde(sigma, ts.cols());

  unsigned T = ts.rows();
  Eigen::MatrixXd distances(T-W, T-W);
  distances.setZero();
  std::cerr << "Computing distances..." << std::endl;

  kde.DistanceMatrix(ts, W, distances);

  std::cout << distances << std::endl;

  return 0;
}
