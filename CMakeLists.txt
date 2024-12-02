# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Compiler_From_Scratch)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Set the custom g++ compiler (replace with your actual path)
set(CMAKE_CXX_COMPILER g++)
# Add source files
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

# Create an executable target
add_executable(MyProject ${SOURCES})
