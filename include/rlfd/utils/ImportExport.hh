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
#ifndef __IMPORT_EXPORT_HH__
#define __IMPORT_EXPORT_HH__

#include <rlfd/utils/Matio.hh>
#include <rlfd/utils/Tabulario.hh>

#include <string>
#include <stdexcept>
#include <Eigen/Core>
#include <iostream>
namespace rlfd {
namespace utils {

void Import(const std::string& filename, Eigen::MatrixXd& out) 
{
  // @TODO Rely on extensions, for now
  std::string extension = filename.substr(filename.find_last_of(".") + 1);

  rlfd::utils::Matrixio* mat; 

  if (extension == "mat") {
    mat = new rlfd::utils::Matio(); 
  } else if (extension == "dat") {
    mat = new rlfd::utils::Tabulario();
  } else {
    throw std::runtime_error(std::string("Unsupported file type ") + extension);
  }

  mat->Open(filename);
  mat->Read(out);
  mat->Close();
}

} // namespace utils
} // namespace rlfd
#endif // __IMPORT_EXPORT_HH__
