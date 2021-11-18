# PROJECT FILES
set(CORE_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/main.cpp"
        "${PROJECT_SOURCE_DIR}/src/img/image.cpp"
        "${PROJECT_SOURCE_DIR}/src/img/pixel.cpp"
        "${PROJECT_SOURCE_DIR}/src/img/file_data.cpp"
        "${PROJECT_SOURCE_DIR}/src/img/process.cpp"
        "${PROJECT_SOURCE_DIR}/src/img/utility.cpp"
        )

add_executable(img ${CORE_SOURCE_FILES})
target_include_directories(img PUBLIC "${PROJECT_SOURCE_DIR}/include/")

# DEPENDENCIES
message(STATUS "Linking STB to project.")
target_link_libraries(img stb)

message(STATUS "Linking GLM to project.")
target_link_libraries(img glm)