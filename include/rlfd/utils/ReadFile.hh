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
#ifndef __READFILE_HH__
#define __READFILE_HH__

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>

namespace rlfd {
namespace utils {

class Line {
  std::string data;
 public:
  friend std::istream &operator>>(std::istream &is, Line &l) {
    std::getline(is, l.data);
    return is;
  }
  operator std::string() const { return data; }    
};

void ReadFile(std::istream& file, std::vector<std::string>& out)
{
  std::copy(std::istream_iterator<Line>(file), 
            std::istream_iterator<Line>(),
            std::back_inserter(out));
}

} // namespace utils
} // namespace rlfd
#endif // __READFILE_HH__

