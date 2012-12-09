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
#include <rlfd/delay/DelayEmbedding.hh>
#include <rlfd/segment/KohlmorgenLemm.hh>
#include <rlfd/stats/GaussianDensityEstimator.hh>

#include <limits>
#include <iostream>

#include <getopt.h>

void print_usage(void)
{
  std::cout << "Execute the Kohlmorgen-Lemm algorithm on the data passed through STDIN" << std::endl;
  std::cout << "Usage: kolmorgen-lemm [OPTION]" << std::endl;
  std::cout << "  -m --dimension    The Embedding dimension." << std::endl;
  std::cout << "  -d --delay        The lag value." << std::endl;
}

int main(int argc, char** argv)
{
  std::cout.precision(std::numeric_limits<double>::digits10);

  int embedding_dimension = 2;
  int lag = 1;
  double W = 50;
  double regularizer = 0;

  // Parse arguments
  static struct option long_options[] =
  {
    {"dimension", required_argument, 0, 'm'},
    {"delay", required_argument, 0, 'd'},
    {"regularizer", required_argument, 0, 'C'},
    {"window", required_argument, 0, 'W'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "m:d:C:W:", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 'm' :
        embedding_dimension = std::stoi(optarg);
        break;
      case 'd' :
        lag = std::stoi(optarg);
        break;
      case 'C' :
        regularizer = std::stod(optarg);
        break;
      case 'W':
        W = std::stoi(optarg);
        break;
      default:
        print_usage();
        return -1;
    }
  }

  Eigen::MatrixXd ts;
  if (optind < argc) {
    std::cout << "Importing from file" << std::endl;
    rlfd::utils::Import(argv[optind], ts);
  } else {
    // Read from stdin
    rlfd::utils::Import(ts);
  }

  // Embed the input points
  Eigen::MatrixXd embTs;
  rlfd::delay::DelayEmbedding::Embed(ts, embedding_dimension, lag, embTs);

  // Estimate the sigma parameter for KDE
  rlfd::stats::GaussianDensityEstimator kde;
  kde.Calibrate(embTs);
  std::cout << "Sigma : " << kde.GetSigma() << std::endl;
  std::cout << "d: " << kde.GetDimensionality() << std::endl;
  std::cout << "W: " << W << std::endl;

  rlfd::segment::KohlmorgenLemm<rlfd::stats::GaussianDensityEstimator> segmenter(kde, W, regularizer);

  for (int t = W; t < embTs.rows(); t++) {
    segmenter.AddObservation(embTs, t);
  }

  return 0;
}
