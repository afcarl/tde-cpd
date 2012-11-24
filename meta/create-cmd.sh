#!/bin/bash

cat <<EOF
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
#include <getopt.h>

void print_usage(void)
{
EOF

BIN_NAME=$1

echo "  std::cout << \"Usage: $BIN_NAME [OPTION] [FILE]\" << std::endl;"
echo "}"
echo ""
echo "int main(int argc, char** argv)"
echo "{"

cat <<EOF
  int nearest_neighbor = 4;
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
        nearest_neighbor = std::stoi(optarg);
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
EOF


echo "  return 0;"
echo "}"

