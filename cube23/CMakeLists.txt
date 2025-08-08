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

# Setup SPIRV shader compilation for cube23
include(FetchContent)
FetchContent_Declare(cmake-spirv
        GIT_REPOSITORY https://github.com/liliolett/cmake-spirv.git
        GIT_TAG origin/v1
)
FetchContent_MakeAvailable(cmake-spirv)

list(APPEND CMAKE_MODULE_PATH ${cmake-spirv_SOURCE_DIR}/include)
include(AddSpirvModules)

add_spirv_modules(cube23_shaders
        SOURCE_DIR assets/shaders
        BINARY_DIR shaders
        SOURCES texture.vert texture.frag)

add_dependencies(cube23 cube23_shaders)

# Copy assets
add_custom_command(TARGET cube23 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders)
add_custom_command(TARGET cube23 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures
    ${CMAKE_CURRENT_BINARY_DIR}/textures)
