project(ComputerVisionAlgorithms)
cmake_minimum_required(VERSION 3.10)

include_directories(${CMAKE_SOURCE_DIR}/dep/stb)

file(GLOB SOURCES src/Image.cpp src/Canny.cpp src/Gaussian.cpp)

add_executable(main ${SOURCES}
					./main.cpp)

target_include_directories(main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
