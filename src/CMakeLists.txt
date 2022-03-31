add_executable(${PROJECT_NAME} 
    ADtest.cpp
    AD/ADVar.hpp
)


target_link_libraries(${PROJECT_NAME} 
    fmt
)