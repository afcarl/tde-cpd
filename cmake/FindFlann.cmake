###############################################################################
# Find Flann
#
# This sets the following variables:
# FLANN_FOUND - True if FLANN was found.
# FLANN_INCLUDE_DIRS - Directories containing the FLANN include files.
# FLANN_LIBRARIES - Libraries needed to use FLANN.
# FLANN_DEFINITIONS - Compiler flags for FLANN.

find_package(PkgConfig)
pkg_check_modules(PC_FLANN flann)
set(FLANN_DEFINITIONS ${PC_FLANN_CFLAGS_OTHER})

find_path(FLANN_INCLUDE_DIR flann/flann.hpp
	HINTS ${PC_FLANN_INCLUDEDIR} ${PC_FLANN_INCLUDE_DIRS})

find_library(FLANN_LIBS flann_cpp
	HINTS ${PC_FLANN_LIBDIR} ${PC_FLANN_LIBS_DIRS})

set(FLANN_INCLUDE_DIRS ${FLANN_INCLUDE_DIR})
set(FLANN_LIBRARIES ${FLANN_LIBS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Flann DEFAULT_MSG
	FLANN_LIBS FLANN_INCLUDE_DIR)

mark_as_advanced(FLANN_LIBS FLANN_INCLUDE_DIR)
