cmake_minimum_required(VERSION 3.10)
project(FiveLang VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory to the project
include_directories(include includes)

# Add all the source files to the project
file(GLOB SOURCES "source/*.cpp" "includes/*.cpp")

# Add the executable to the project
add_executable(FiveLang ${SOURCES})