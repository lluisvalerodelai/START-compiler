# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(Compiler_From_Scratch)

# Set the C++ standard (optional, but recommended)
set(CMAKE_CXX_STANDARD 17) 

# Find the include directory
include_directories(include)

# Set the custom g++ compiler (replace with your actual path)
set(CMAKE_CXX_COMPILER g++)

# Add your source files
file(GLOB SOURCE_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link libraries (if needed)
# target_link_libraries(MyProject some_library) 