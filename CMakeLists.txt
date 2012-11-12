cmake_minimum_required(VERSION 2.6)

PROJECT( rlfd_segmentation )

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wno-enum-compare -g -Wall")
endif()

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(Flann REQUIRED)
include_directories(${FLANN_INCLUDE_DIR})

find_package(FFTW REQUIRED)
include_directories(${FFTW_INCLUDES})

find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)

include_directories(${CMAKE_SOURCE_DIR}/external/odeint)

ADD_EXECUTABLE(kohlmorgen-lemm src/KohlmorgenLemm.cc)
TARGET_LINK_LIBRARIES(kohlmorgen-lemm ${FLANN_LIBS} ${FFTW_LIBRARIES})

ADD_EXECUTABLE(autocorrelation src/Autocorrelation.cc)
TARGET_LINK_LIBRARIES(autocorrelation ${FFTW_LIBRARIES} "-lmatio -lz")

ADD_EXECUTABLE(average-displacement src/AverageDisplacement.cc)
TARGET_LINK_LIBRARIES(average-displacement ${FFTW_LIBRARIES} "-lmatio -lz")

ADD_EXECUTABLE(gamma-test src/GammaTest.cc)
TARGET_LINK_LIBRARIES(gamma-test ${FLANN_LIBS} "-lmatio -lz")

ADD_EXECUTABLE(delay-embedding src/DelayEmbedding.cc)
TARGET_LINK_LIBRARIES(delay-embedding "-lmatio -lz")

ADD_EXECUTABLE(increasing-embedding src/IncreasingEmbedding.cc)
TARGET_LINK_LIBRARIES(increasing-embedding "-lmatio -lz")

ADD_EXECUTABLE(mattodat src/MatToDat.cc)
TARGET_LINK_LIBRARIES(mattodat "-lmatio -lz")

ADD_EXECUTABLE(lorenz src/Lorenz.cc)
