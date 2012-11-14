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
#include <rlfd/delay/AutomatedEmbedding.hh>

#include <string>
#include <limits>
#include <iostream>

#include <getopt.h>

void print_usage(void)
{
  std::cout << "Usage: " << "automated-embedding " << "[OPTION] [FILE]" << std::endl;
  std::cout << "Usage: " << std::endl;
  std::cout << "  -h --help               This help message." << std::endl;
  std::cout << "  -L --max-lag            Maximum number of lags to compute in the average displacement method." << std::endl;
  std::cout << "  -M --max-dimension      Maximum dimension" << std::endl;
  std::cout << "  -n --nearest-neighbors  Number of nearest neighbors to compute in the gamma test." << std::endl;
}

int main(int argc, char** argv)
{
  // Default values
  int max_dimension = 10;
  int max_lag = 50;
  int nn = 20;

  // Parse arguments
  static struct option long_options[] =
  {
    {"help", no_argument, 0, 'h'},
    {"max-lag", required_argument, 0, 'L'},
    {"max-dimension", required_argument, 0, 'M'},
    {"nearest-neighbors", no_argument, 0, 'n'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "hL:M:n:", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 'M':
        max_dimension = std::stoi(optarg);
        break;
      case 'L' :
        max_lag = std::stoi(optarg);
        break;
      case 'n':
        nn = std::stoi(optarg); 
        break;
      case 'h':
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


  // Print the sm statistics
  std::cout.precision(std::numeric_limits<double>::digits10);
  std::cout << rlfd::delay::AutomatedEmbedding(ts, max_lag, max_dimension, nn) << std::endl; 
  return 0;
}
