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
#ifndef __TABULARIO_HH__
#define __TABULARIO_HH__

#include <rlfd/utils/Matrixio.hh>

#include <tuple>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

namespace rlfd {
namespace utils {

class Tabulario : public Matrixio
{
 public:
  Tabulario() {};
  virtual ~Tabulario() {};

 private:
  std::ifstream file;

  void split_line(const std::string& line, std::vector<std::string>& words)
  {
    std::istringstream iss(line);
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter<std::vector<std::string>>(words));
  }

  std::tuple<int, int> get_dimensions(const std::vector<std::string>& lines)
  {
    std::vector<std::string> cols;
    split_line(lines[0], cols);
    return std::make_tuple(lines.size(), cols.size());
  }

  class Line {
    std::string data;
   public:
    friend std::istream &operator>>(std::istream &is, Line &l) {
      std::getline(is, l.data);
      return is;
    }
    operator std::string() const { return data; }    
  };

 public:
  void Open(const std::string& filename) throw(std::runtime_error)
  { 
    file.open(filename, std::ifstream::in);
  }

  void Read(Eigen::MatrixXd& out) throw(std::runtime_error)
  {
    // Read all lines
    std::vector<std::string> lines;
    std::copy(std::istream_iterator<Line>(file), 
              std::istream_iterator<Line>(),
              std::back_inserter(lines));

    // Find matrix dimensions
    auto dims = get_dimensions(lines);
    out.resize(std::get<0>(dims), std::get<1>(dims));

    // Copy each element to the matrix
    int i = 0;
    for (auto line : lines) {
      std::vector<std::string> tokens; 
      split_line(line, tokens);

      int j = 0;
      for (auto element : tokens) {
        out(i, j) = std::stod(element);
        j += 1;
      }
      i += 1;
    }
  } 

  void Close(void) throw(std::runtime_error)
  {
    file.close();
  }
};

} // namespace utils
} // namespace rlfd
#endif // __TABULARIO_HH__
