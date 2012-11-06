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
#include <rlfd/delay/AverageDisplacement.hh>
#include <rlfd/delay/SquaredAverageDisplacement.hh>

#include <string>
#include <iostream>

#include <getopt.h>

void print_usage(void)
{
  std::cerr << "Usage: " << "average-displacement" << "[OPTION] [Embedding Dimension] [FILE]" << std::endl;
  std::cerr << "  -n --nlags        The number of lags to compute." << std::endl;
  std::cerr << "  -p --plot         Plot S_m^2 as a function of tau" << std::endl;
  std::cerr << "  -r --rate         Sampling rate" << std::endl;
  std::cerr << "  -s --squared      Compute the average squared using the sample autocorrelation function" << std::endl;
}

int main(int argc, char** argv)
{
  // Default values
  int lag = 20;
  bool plot = false;
  double rate = 0.0;
  bool squared = false;

  // Parse arguments
  static struct option long_options[] =
  {
    {"nlags", required_argument, 0, 'n'},
    {"plot", no_argument, 0, 'p'},
    {"rate", required_argument, 0, 'r'},
    {"squared", no_argument, 0, 's'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "n:pr:s", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 'n' :
        lag = std::stoi(optarg);
        break;
      case 'p':
        plot = true;
        break;
      case 'r':
        rate = std::stod(optarg);
        break;
      case 's':
        squared = true;
        break;
      default:
        print_usage();
        return -1;
    }
  }

  if (!(optind + 2 <= argc)) {
     print_usage();
     return -1;
  }

  int embedding_dimension = std::stoi(argv[optind++]);
  std::string filename = argv[optind];

  // Import the matrix
  Eigen::MatrixXd ts;
  rlfd::utils::Import(filename, ts); 

  // Compute the statistics for a range of tau values
  Eigen::VectorXd ads;
  if (squared) {
    std::cout << "# Squared average displacement statistics" << std::endl;
    ads = rlfd::delay::SquaredAverageDisplacement(ts.col(1), embedding_dimension, lag);
  } else {
    std::cout << "# Average displacement statistics" << std::endl;
    ads = rlfd::delay::AverageDisplacement(ts.col(1), embedding_dimension, lag);
  }

  if (plot) {
    rlfd::utils::Gnuplot gnuplot;
    gnuplot(ads);
  }

  // Scale the statistics on 0 to 1 range
  double maxCoeff = ads.maxCoeff();
  double minCoeff = ads.minCoeff();
  ads.array() = (ads.array() - minCoeff).array()/(maxCoeff - minCoeff); 

  double threshold = 1.0/std::exp(1.0);
  for (int i = 0; i < ads.size(); i++) {
    if (ads[i] > threshold) {
      std::cout << "tau = ";
      if (rate) {
        std::cout << i*rate << " sec" << std::endl;
      } else {
        std::cout << i << " t/s" << std::endl;
      }
      break;
    }
  }

  //std::cout << ads << std::endl;

  return 0;
}
