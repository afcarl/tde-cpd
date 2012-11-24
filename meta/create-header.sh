#!/bin/bash

expand_namespaces ()
{
  local NAMESPACE=$1

  if [ $NAMESPACE ]
  then
    echo "namespace $NAMESPACE {"

    shift
    expand_namespaces $@

    echo "} // namespace $NAMESPACE"
  else
    echo ""
  fi
}

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
EOF

FILENAME=$1
shift

echo "#ifndef __${FILENAME^^}_HH__"
echo "#define __${FILENAME^^}_HH__"
echo ""

expand_namespaces $@

echo "#endif // __${FILENAME^^}_HH__"
echo ""
