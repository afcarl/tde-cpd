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

#include <Eigen/Core>
#include <flann/flann.hpp>

#include <getopt.h>

void print_usage(void)
{
  std::cout << "Build a Kd-Tree index for the input data using libflann" << std::endl;
  std::cout << "Usage: build-kdtree [OPTION] [FILE]" << std::endl;
  std::cout << "FILE is the mandatory output filename. The data to embed is expected to be received from STDIN." << std::endl;
}

int main(int argc, char** argv)
{
  static struct option long_options[] =
  {
    {0, 0, 0, 0}
  };

  int option_index = 0;
  int c;
  while ((c = getopt_long(argc, argv, "", long_options, &option_index)) != -1)
  {
    switch (c)
    {
      default:
        print_usage();
        return -1;
    }
  }

  // Treat the non-option as the FILE argument
  if (optind >= argc) {
    std::cerr << "The output FILE argument must be provided.\n" << std::endl;
    print_usage();
    return -1;
  }

  // Read from stdin
  Eigen::MatrixXd in;
  rlfd::utils::Import(in);

  flann::Matrix<double> input(new double[in.rows()*in.cols()], in.rows(), in.cols());
  for (int i = 0; i  < in.rows(); i++) {
    for (int j = 0; j < in.cols(); j++) {
      input[i][j] = in(i, j);
    }
  }

  flann::Index<flann::L2<double> > index(input, flann::KDTreeSingleIndexParams());
  index.buildIndex();
  index.save(argv[optind]);

  return 0;
}
