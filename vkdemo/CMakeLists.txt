cmake_minimum_required(VERSION 3.26)
project(vkdemo)

set(CMAKE_CXX_STANDARD 20)

set(VkDemo_DIR src)
set(VkDemo_SOURCES
        src/main.cpp
)

find_package(Vulkan REQUIRED)

add_executable(vkdemo ${VkDemo_SOURCES})
target_include_directories(vkdemo PRIVATE 
    ${Vulkan_INCLUDE_DIRS}
    ../vox/vendor/glfw/include
    ../vox/vendor/glm
    ../vox/vendor/stb/include
)
target_link_libraries(vkdemo PRIVATE 
    glfw 
    ${Vulkan_LIBRARIES}
)

# Setup SPIRV shader compilation for vkdemo
include(FetchContent)
FetchContent_Declare(cmake-spirv
        GIT_REPOSITORY https://github.com/liliolett/cmake-spirv.git
        GIT_TAG origin/v1
)
FetchContent_MakeAvailable(cmake-spirv)

list(APPEND CMAKE_MODULE_PATH ${cmake-spirv_SOURCE_DIR}/include)
include(AddSpirvModules)

add_spirv_modules(vkdemo_shaders
        SOURCE_DIR assets/shaders
        BINARY_DIR shaders
        SOURCES shader.vert shader.frag)

add_dependencies(vkdemo vkdemo_shaders)

# Copy assets
add_custom_command(TARGET vkdemo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/assets/textures
    ${CMAKE_CURRENT_BINARY_DIR}/textures)
