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

include_directories(${CMAKE_SOURCE_DIR}/external/odeint)

include_directories(${CMAKE_SOURCE_DIR}/include)

ADD_EXECUTABLE(ts-segment src/TimeSeriesSegmentation.cc)
TARGET_LINK_LIBRARIES(ts-segment ${FLANN_LIBS} ${FFTW_LIBRARIES} ${BOOST_LIBRARIES})

ADD_EXECUTABLE(average-displacement src/AverageDisplacement.cc)
TARGET_LINK_LIBRARIES(average-displacement ${FLANN_LIBS} ${FFTW_LIBRARIES} "-lmatio -lz")

ADD_EXECUTABLE(autocorrelation src/Autocorrelation.cc)
TARGET_LINK_LIBRARIES(autocorrelation ${FFTW_LIBRARIES} "-lmatio -lz")

ADD_EXECUTABLE(lorenz src/Lorenz.cc)

ADD_EXECUTABLE(catmat src/CatMat.cc)
TARGET_LINK_LIBRARIES(catmat "-lmatio -lz")
