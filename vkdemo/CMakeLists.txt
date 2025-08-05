set(VkDemo_SOURCES
        src/main.cpp
)

add_executable(vkdemo ${VkDemo_SOURCES})
target_include_directories(vkdemo PRIVATE ${Vox_DIR})
target_link_libraries(vkdemo PRIVATE vox ${Vulkan_LIBRARIES})

add_dependencies(vkdemo shaders)

# Copy textures directory to build directory (shaders are already compiled to shaders/ by add_spirv_modules)
add_custom_command(TARGET vkdemo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/cube23/assets/textures
    ${CMAKE_CURRENT_BINARY_DIR}/textures)
