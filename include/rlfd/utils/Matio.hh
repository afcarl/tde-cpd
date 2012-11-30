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
#ifndef __MATIO_HH__
#define __MATIO_HH__

#include <rlfd/utils/Matrixio.hh>
#include <matio.h>

namespace rlfd {
namespace utils {

template<typename MatrixType=Eigen::MatrixXd>
class Matio : public Matrixio<MatrixType>
{
 public:
  Matio() : fp_(NULL) {};
  virtual ~Matio() {};

  void Open(const std::string& filename) throw(std::runtime_error)
  {
    fp_ = Mat_Open(filename.c_str(), MAT_ACC_RDONLY);
    if (fp_ == NULL) {
      throw std::runtime_error("Failed to open mat file");
    }
  }

  void Read(MatrixType& out) throw(std::runtime_error)
  {
    // @FIXME Just read the first variable instead of "y"
    Read(std::string("y"), out);
  }

  void Read(const std::string& name, MatrixType& out) throw(std::runtime_error)
  {
    matvar_t* matvar = Mat_VarRead(fp_, const_cast<char*>(name.c_str()));
    if (matvar == NULL) {
      throw std::runtime_error(std::string("Failed to read variable") + name);
    }

    out = MatrixType::Map((double*) matvar->data, matvar->dims[0], matvar->dims[1]);

    Mat_VarFree(matvar);
  }

  void Close(void) throw(std::runtime_error)
  {
    Mat_Close(fp_);
  }

private:
  mat_t* fp_;

};

} // namespace rlfd
} // namespace utils

#endif // __MATIO_HH__
