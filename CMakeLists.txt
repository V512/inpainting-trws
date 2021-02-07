cmake_minimum_required(VERSION 3.10)

project(TRWS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED )

add_executable(TRWS 
                main.cpp 
                neighborStruct.cpp neighborStruct.h 
                phi.h phi.cpp 
                timeMeasurement.h
                chainsWeight.h chainsWeight.cpp 
                trwsInpainting.cpp trwsInpainting.h 
                )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/)

link_directories( ${CMAKE_BINARY_DIR}/bin)

target_link_libraries( TRWS ${OpenCV_LIBS} )