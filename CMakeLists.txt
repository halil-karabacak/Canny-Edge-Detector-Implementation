project(ComputerVisionAlgorithms)
cmake_minimum_required(VERSION 3.25)

set (OpenCV_DIR "D:/OpenCV/build/")
find_package(OpenCV REQUIRED)

file(GLOB SOURCES src/*.cpp include/*.h)

add_executable(main ${SOURCES}
					./main.cpp)


target_include_directories(main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "dep/eigen-3.4.0")

target_link_libraries(main ${OpenCV_LIBS})