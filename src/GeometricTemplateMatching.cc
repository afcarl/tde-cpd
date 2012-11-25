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
#include <rlfd/utils/ReadDir.hh>
#include <rlfd/utils/ImportExport.hh>
#include <rlfd/delay/DelayEmbedding.hh>
#include <rlfd/delay/GeometricTemplateMatching.hh>

#include <limits>
#include <iostream>

#include <getopt.h>

void print_usage(void)
{
  std::cout << "Geometric Template Matching (GeTM) algorithm.\n\
Originally described in: \n\
Activity and Gait Recognition with Time-Delay Embeddings (AAAI 2010), \n\
with S. Mannor and D. Precup, July 2010.\n\
\n\
Compare the test sequence given as STDIN to the base models under DIR.\n\
The geometric template matching algorithm is then applied upon each\n\
of the base models and the similarity scores sent to STDOUT.\n\
\n\
Usage: getem [OPTION] [DIR]\n\
  -s  --segment-length    Segment length. Default 32\n\
  -n  --nearest-neighbor  Number of nearest neighbors. Default 4" << std::endl;
}

int main(int argc, char** argv)
{
  // Default values
  int segment_length = 32;
  int nearest_neighbor = 4;

  // Parse arguments
  static struct option long_options[] =
  {
    {"segment-length", required_argument, 0, 's'},
    {"nearest-neighbor", required_argument, 0, 'n'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "s:n:", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 's':
        segment_length = std::stoi(optarg);
        break;
      case 'n':
        nearest_neighbor = std::stoi(optarg);
        break;
      default:
        print_usage();
        return -1;
    }
  }

  // Treat the non-option as the FILE argument
  Eigen::MatrixXd testEmb;
  if (optind >= argc) {
    std::cerr << "Unspecified DIR argument." << std::endl << std::endl;
    print_usage();
    return -1;
  }

  // Import the embedded test sequence from STDIN
  rlfd::utils::Import(testEmb);

  // Import the base model from FILE
  Eigen::MatrixXd modelEmb;
  rlfd::utils::Import(argv[optind], modelEmb);

  rlfd::delay::DelayEmbedding model;
  model.SetMatrix(modelEmb);
  model.BuildIndex();

  Eigen::VectorXd r;
  rlfd::delay::GeometricTemplateMatching(model, testEmb, r, segment_length, nearest_neighbor);
  std::cout.precision(std::numeric_limits<double>::digits10);
  std::cout << r << std::endl;

  return 0;
}
