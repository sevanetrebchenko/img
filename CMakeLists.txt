cmake_minimum_required(VERSION 3.0)

# Project information.
project(itoa
        VERSION 1.0
        DESCRIPTION "Image to Ascii"
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# Add STB as a library.
add_library(stb INTERFACE)

# Anything that targets stb will need to see the directories for STB includes.
target_include_directories(stb INTERFACE "${PROJECT_SOURCE_DIR}/lib/stb")


# PROJECT FILES
set(CORE_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/main.cpp"
)

add_executable(itoa ${CORE_SOURCE_FILES})

# DEPENDENCIES
# Link STB
message(STATUS "Linking STB to itoa project.")
target_link_libraries(itoa stb)





