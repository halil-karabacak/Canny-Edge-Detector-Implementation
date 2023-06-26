project(ComputerVisionAlgorithms)
cmake_minimum_required(VERSION 3.25)

file(GLOB SOURCES src/*.cpp include/*.h)

add_executable(main ${SOURCES}
					./main.cpp)


target_include_directories(main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
