cmake_minimum_required (VERSION 2.8)
project (estimation)

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")

find_package (OpenCL REQUIRED)

include_directories (${OPENCL_INCLUDE_DIRS})

add_executable (${PROJECT_NAME} main.cpp)

target_link_libraries (${PROJECT_NAME} ${OPENCL_LIBRARIES})