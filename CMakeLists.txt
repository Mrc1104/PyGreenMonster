cmake_minimum_required(VERSION 3.31)
project(GreenMonster VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(functions)

set(PYTHON_LIBRARY_DIR "${CMAKE_SOURCE_DIR}/Bindings")
file(MAKE_DIRECTORY ${PYTHON_LIBRARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PYTHON_LIBRARY_DIR}")

add_subdirectory(RemexClient)
add_subdirectory(BMW)
add_subdirectory(EPICS)
add_subdirectory(Queue)
