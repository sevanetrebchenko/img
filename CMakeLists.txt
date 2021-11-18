cmake_minimum_required(VERSION 3.0)

# Project information.
project(img
        VERSION 1.0
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# Build third-party dependencies.
message(STATUS "--- Adding lib subdirectory ---")
add_subdirectory(lib)

# Build project sources.
message(STATUS "--- Adding src subdirectory ---")
add_subdirectory(src)





