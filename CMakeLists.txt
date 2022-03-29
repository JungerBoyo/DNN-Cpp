cmake_minimum_required(VERSION 3.15.0)
project(VkGradientDescent)

set(CMAKE_CXX_STANDARD 20)

include_directories(
    src/ 
    src/AD
)


add_executable(${PROJECT_NAME} 
    src/ADtest.cpp
    src/AD/ADVar.hpp
)


target_link_libraries(${PROJECT_NAME} 
    fmt
)