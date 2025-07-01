# Sets the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 3.10)

# Declares the project name.
project(code_counter)

# Sets the build type to Debug, which includes debugging information in the executable.
set(CMAKE_BUILD_TYPE Debug)

# Sets the C++ standard to C++17.
set(CMAKE_CXX_STANDARD 17)

# Adds an executable target named "code_counter" which is built from the specified source files.
add_executable(code_counter main.cpp fileparser.cpp)

# Add testing framework
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)

FetchContent_MakeAvailable(googletest)

add_executable(tests tests/test_fileparser.cpp fileparser.cpp)
target_link_libraries(tests PRIVATE gtest_main)

add_test(NAME unit_tests COMMAND tests)