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
#ifndef __READIR_HH__
#define __READIR_HH__

#include <sys/types.h>
#include <dirent.h>
#include <string.h>

#include <vector>
#include <string>
#include <iterator>
#include <stdexcept>

namespace rlfd {
namespace utils {

/**
 * List the files under a directory
 * @param path The path to the directory to be listed
 * @param out An output iterator to an stl container
 */
template<class OutputIterator>
void ReadDir(const std::string& path, OutputIterator container)
{
  DIR *dir;
  struct dirent *entry;

  if ((dir = opendir("./meta")) == NULL) {
    throw std::runtime_error(strerror(errno));
  } else {
    while ((entry = readdir(dir)) != NULL) {
      *container++ = std::string(entry->d_name);
    }
    closedir(dir);
  }
}

} // namespace utils
} // namespace rlfd
#endif // __READIR_HH__

