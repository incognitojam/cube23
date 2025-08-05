cmake_minimum_required(VERSION 3.26)
project(cube23)

set(CMAKE_CXX_STANDARD 20)

set(Cube_DIR src)
set(Cube_SOURCES
        src/cube_app.cpp
)

add_executable(cube23 ${Cube_SOURCES})
target_include_directories(cube23 PRIVATE ../vox/src)
target_link_libraries(cube23 PRIVATE vox)

# Copy assets
add_custom_command(TARGET cube23 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders)
add_custom_command(TARGET cube23 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures
    ${CMAKE_CURRENT_BINARY_DIR}/textures)
