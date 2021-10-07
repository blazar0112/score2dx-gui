cmake_minimum_required(VERSION 3.15)

# Disable in-source build.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source build is not allowed. Remember to remove CMakeCache.txt and CMakeFiles/")
endif()

project(Score2dx_GUI
    DESCRIPTION "Score2dx GUI."
    LANGUAGES CXX
)

add_subdirectory(src)
