cmake_minimum_required(VERSION 3.0)

# Project information.
project(img
        VERSION 1.0
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# Build third-party dependencies.
message(STATUS "--- Adding library subdirectory ---")
add_subdirectory(lib)

# PROJECT FILES
set(CORE_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/main.cpp"
        "${PROJECT_SOURCE_DIR}/src/image.cpp"
        "${PROJECT_SOURCE_DIR}/src/utility.cpp"
)

add_executable(img ${CORE_SOURCE_FILES})
target_include_directories(img PUBLIC "${PROJECT_SOURCE_DIR}/include/")

# DEPENDENCIES
# Link FreeType
message(STATUS "Linking STB to project.")
target_link_libraries(img stb)

message(STATUS "Linking GLM to project.")
target_link_libraries(img glm)

# Link STB
message(STATUS "Linking FreeType to project.")
target_link_libraries(img freetype)





