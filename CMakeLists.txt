cmake_minimum_required(VERSION 3.15.0)
project(VkGradientDescent)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    src/ 
    src/AD
)

add_subdirectory(src)