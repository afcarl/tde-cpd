# Try to find FFTW
# adapted from FINDEIGEN.cmake of OpenStructure by Juergen Haas <juergen.haas@unibas.ch>
# license see FINDEIGEN.cmake 
#
# - Find FFTW
# Find the native FFTW includes and library
# 
#  FFTW_INCLUDE_DIR - where to find gsl/gsl_*.h, etc.
#  FFTW_LIBS   - List of libraries when using FFTW.
#  FFTW_FOUND       - True if FFTW found.

if (FFTW_INCLUDE_DIR)
 set(FFTW_FOUND TRUE)
else (FFTW_INCLUDE_DIR)
  find_path (FFTW_INCLUDE_DIR fftw3.h)
  if (_DOUBLE_PREC)
    find_library (FFTW_LIBS NAMES fftw3)
    find_library (FFTW_THREADS_LIB NAMES fftw3_threads)
  elseif(NOT _DOUBLE_PREC)
    find_library (FFTW_LIBS NAMES fftw3f)
    find_library (FFTW_THREADS_LIB NAMES fftw3f_threads)
  endif()
  if(FFTW_THREADS_LIB)
    SET(FFTW_USE_THREADS TRUE)
    SET(FFTW_LIBS ${FFTW_LIBS} ${FFTW_THREADS_LIB} )
  else(FFTW_THREADS_LIB)
  endif(FFTW_THREADS_LIB)
endif (FFTW_INCLUDE_DIR)




# handle the QUIETLY and REQUIRED arguments and set FFTW_FOUND to TRUE if 
# all listed variables are TRUE
include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (FFTW DEFAULT_MSG FFTW_LIBS FFTW_INCLUDE_DIR)

mark_as_advanced (FFTW_LIBS FFTW_INCLUDE_DIR)
