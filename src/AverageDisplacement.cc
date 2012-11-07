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
#include <rlfd/delay/AverageDisplacement.hh>
#include <rlfd/delay/SquaredAverageDisplacement.hh>

#include <string>
#include <limits>
#include <iostream>

#include <getopt.h>

void print_usage(void)
{
  std::cerr << "Usage: " << "average-displacement" << "[OPTION] [Embedding Dimension] [FILE]" << std::endl;
  std::cerr << "  -m --dimension    Embedding dimension" << std::endl;
  std::cerr << "  -n --nlags        Number of lags to compute." << std::endl;
  std::cerr << "  -s --squared      Compute the average squared using the sample autocorrelation function" << std::endl;
}

int main(int argc, char** argv)
{
  // Default values
  int embedding_dimension = 2;
  int lag = 20;
  bool squared = false;

  // Parse arguments
  static struct option long_options[] =
  {
    {"dimension", required_argument, 0, 'm'},
    {"nlags", required_argument, 0, 'n'},
    {"squared", no_argument, 0, 's'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "m:n:s", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 'm':
        embedding_dimension = std::stoi(optarg);
        break;
      case 'n' :
        lag = std::stoi(optarg);
        break;
      case 's':
        squared = true;
        break;
      default:
        print_usage();
        return -1;
    }
  }

  // Treat the non-option as the FILE argument
  Eigen::MatrixXd ts;
  if (optind < argc) {
    rlfd::utils::Import(argv[optind], ts);
  } else {
    // Read from stdin
    rlfd::utils::Import(ts);
  }

  // Compute the statistics for a range of tau values
  Eigen::VectorXd ads;
  if (squared) {
    std::cout << "# Squared average displacement statistics" << std::endl;
    ads = rlfd::delay::SquaredAverageDisplacement(ts, embedding_dimension, lag);
  } else {
    std::cout << "# Average displacement statistics" << std::endl;
    ads = rlfd::delay::AverageDisplacement(ts, embedding_dimension, lag);
  }

  // Scale the statistics on 0 to 1 range
  double maxCoeff = ads.maxCoeff();
  double minCoeff = ads.minCoeff();
  ads.array() = (ads.array() - minCoeff).array()/(maxCoeff - minCoeff); 

  // Print the sm statistics
  std::cout.precision(std::numeric_limits<double>::digits10);
  std::cout << ads << std::endl;

  return 0;
}
