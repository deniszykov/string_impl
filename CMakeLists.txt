cmake_minimum_required(VERSION 3.10)
project(StringImpl VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Add executable
add_executable(string_impl src/string_impl.cpp)

# Install target
install(TARGETS string_impl DESTINATION bin)