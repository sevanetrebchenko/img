
set(STB_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/lib/stb/stb_image.cpp"
        "${PROJECT_SOURCE_DIR}/lib/stb/stb_image_write.cpp"
        )

add_library(stb STATIC "${STB_SOURCE_FILES}")
target_include_directories(stb PUBLIC "${PROJECT_SOURCE_DIR}/lib/stb")
