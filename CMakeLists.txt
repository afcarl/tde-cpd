cmake_minimum_required(VERSION 2.6)
PROJECT( rlfd_segmentation )

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-g -pg -lpthread -std=c++11 -Wno-enum-compare -Wall")
    set(CMAKE_SHARED_LINKER_FLAGS "-pg")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

find_package(Flann REQUIRED)
include_directories(${FLANN_INCLUDE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/include)

ADD_EXECUTABLE(run_segmentation src/RunSegmentation.cc)
TARGET_LINK_LIBRARIES(run_segmentation ${FLANN_LIBS})
