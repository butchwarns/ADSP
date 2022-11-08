cmake_minimum_required(VERSION 3.10)

# Set project name
project(Audio_DSP_test)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add Catch2 directory (git submodule)
add_subdirectory(Catch2)

# Add executable and link libraries
add_executable(unit_tests test.cpp)
target_link_libraries(unit_tests PRIVATE Catch2::Catch2WithMain)

# Discover tests from Catch2 within CTest (for GitHub actions)
include(CTest)
include(Catch)
catch_discover_tests(unit_tests)
