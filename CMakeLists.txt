cmake_minimum_required(VERSION 2.6)
PROJECT( rlfd_segmentation )

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(Eigen3 REQUIRED)
include_directories(EIGEN3_INCLUDE_DIR)

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-g -pg -lpthread -std=gnu++0x -Wall")
    set(CMAKE_SHARED_LINKER_FLAGS "-pg")
endif()

ADD_LIBRARY(SegDHMM
    src/SegDHMM.cpp)
TARGET_LINK_LIBRARIES(SegDHMM ${EIGEN3_LIBS})

ADD_EXECUTABLE(run_segmentation src/RunSegmentation.cpp)
TARGET_LINK_LIBRARIES(run_segmentation ${EIGEN3_LIBS} SegDHMM)

