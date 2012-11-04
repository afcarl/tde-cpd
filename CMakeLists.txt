cmake_minimum_required(VERSION 2.6)
PROJECT( rlfd_segmentation )

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++11 -Wno-enum-compare -g -Wall")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(Flann REQUIRED)
include_directories(${FLANN_INCLUDE_DIR})

find_package(FFTW REQUIRED)
include_directories(${FFTW_INCLUDES})

find_package(Boost REQUIRED)
include_directories(${BOOST_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/external/odeint)

include_directories(${CMAKE_SOURCE_DIR}/include)

ADD_EXECUTABLE(run_segmentation src/RunSegmentation.cc)
TARGET_LINK_LIBRARIES(run_segmentation ${FLANN_LIBS} ${FFTW_LIBRARIES} ${BOOST_LIBRARIES})

ADD_EXECUTABLE(test_autocorrelation src/TestAutocorrelation.cc)
TARGET_LINK_LIBRARIES(test_autocorrelation ${FLANN_LIBS} ${FFTW_LIBRARIES} "-lmatio -lz")

ADD_EXECUTABLE(run_averagedisplacement src/RunAverageDisplacement.cc)
TARGET_LINK_LIBRARIES(run_averagedisplacement ${FLANN_LIBS} ${FFTW_LIBRARIES} "-lmatio -lz")

ADD_EXECUTABLE(lorenz src/Lorenz.cc)
