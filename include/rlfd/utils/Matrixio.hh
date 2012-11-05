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
#ifndef __MATRIX_IO_HH__
#define __MATRIX_IO_HH__

#include <string>
#include <stdexcept>
#include <Eigen/Core>

namespace rlfd {
namespace utils {

class Matrixio
{
  public:
   Matrixio() {};
   virtual ~Matrixio() {};
   
   virtual void Open(const std::string& filename) throw(std::runtime_error) = 0; 
   virtual void Read(Eigen::MatrixXd& out) throw(std::runtime_error) = 0;
   virtual void Close(void) throw(std::runtime_error) = 0;
};

} // namespace utils
} // namespace rlfd
#endif // __MATRIX_IO_HH__
