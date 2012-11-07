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
#include <rlfd/delay/GammaTest.hh>
#include <rlfd/utils/ImportExport.hh>

#include <limits>
#include <iostream>

#include <getopt.h>

void print_usage(void)
{
    std::cerr << "Compute the gamma test." << std::endl;
    std::cerr << "Usage: gamma-test [OPTION] [FILE]" << std::endl;
    std::cerr << "  -n --nearest-neighbor  The number of nearest neighbors over which to" << std::endl;
    std::cerr << "                         compute the gamma-test statistics." << std::endl;
    std::cerr << "The last column is assumed to be the scalar output time series y." << std::endl;
}

int main(int argc, char** argv)
{
  int nn = 20;

  // Parse arguments
  static struct option long_options[] =
  {
    {"nearest-neighbor", required_argument, 0, 'n'},
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "n:", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      case 'n' :
        nn = std::stoi(optarg);
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

  // Compute the gamma test
  std::cout.precision(std::numeric_limits<double>::digits10);
  std::cout << rlfd::delay::GammaTest(ts.leftCols(ts.cols()-1), ts.rightCols(1), nn) << std::endl;
}
