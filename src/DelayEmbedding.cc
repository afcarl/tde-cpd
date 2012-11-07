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
#include <rlfd/delay/DelayEmbedding.hh>
#include <rlfd/utils/ImportExport.hh>

#include <Eigen/Core>

#include <limits>
#include <iostream>

#include <getopt.h>

void print_usage(void)
{
  std::cerr << "Transform a scalar time series into delay vectors of dimension m." << std::endl;
  std::cerr << "Usage: delay-embedding [OPTION] [FILE]" << std::endl;
  std::cerr << "  -m --dimension    The Embedding dimension." << std::endl;
  std::cerr << "  -l --lag          The lag value." << std::endl; 
}

int main(int argc, char** argv)
{
  int embedding_dimension = 2;
  int lag = 1;

  // Parse arguments
  static struct option long_options[] =
  {
    {"dimension", required_argument, 0, 'm'},
    {"lag", required_argument, 0, 'l'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "m:l:", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 'm' :
        embedding_dimension = std::stoi(optarg);
        break;
      case 'l' :
        lag = std::stoi(optarg);
        break;
      default:
        print_usage();
        return -1;
    }
  }

  Eigen::MatrixXd ts;
  if (optind < argc) {
    rlfd::utils::Import(argv[optind], ts);
  } else {
    // Read from stdin
    rlfd::utils::Import(ts);
  }

  // Delay embedding
  Eigen::MatrixXd out;
  rlfd::delay::DelayEmbedding::Embed(ts, embedding_dimension, lag, out); 
  std::cout.precision(std::numeric_limits<double>::digits10);
  std::cout << out << std::endl;

  return 0;
}
