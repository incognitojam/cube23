cmake_minimum_required(VERSION 3.26)
project(cube23)

set(CMAKE_CXX_STANDARD 20)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vox/vendor/glfw)

add_subdirectory(vox/vendor/glad)

add_subdirectory(vox/vendor/glm)

add_subdirectory(vox/vendor/stb)

set(Vox_DIR vox/src)
set(Vox_SOURCES
        vox/src/vox/core/timestep.h
        vox/src/vox/events/event.h
        vox/src/vox/events/key_event.h
        vox/src/vox/events/mouse_event.h
        vox/src/vox/renderer/buffer.cpp
        vox/src/vox/renderer/buffer.h
        vox/src/vox/renderer/graphics_context.h
        vox/src/vox/renderer/orthographic_camera.cpp
        vox/src/vox/renderer/orthographic_camera.h
        vox/src/vox/renderer/render_command.cpp
        vox/src/vox/renderer/render_command.h
        vox/src/vox/renderer/renderer.cpp
        vox/src/vox/renderer/renderer.h
        vox/src/vox/renderer/renderer_api.cpp
        vox/src/vox/renderer/renderer_api.h
        vox/src/vox/renderer/shader.cpp
        vox/src/vox/renderer/shader.h
        vox/src/vox/renderer/texture.cpp
        vox/src/vox/renderer/texture.h
        vox/src/vox/renderer/vertex_array.cpp
        vox/src/vox/renderer/vertex_array.h
        vox/src/vox/application.cpp
        vox/src/vox/application.h
        vox/src/vox/core.h
        vox/src/vox/entry_point.h
        vox/src/vox/input.h
        vox/src/vox/key_codes.h
        vox/src/vox/mouse_button_codes.h
        vox/src/vox/window.cpp
        vox/src/vox/window.h
        vox/src/Vox.h

        vox/src/platform/opengl/buffer.cpp
        vox/src/platform/opengl/buffer.h
        vox/src/platform/opengl/context.cpp
        vox/src/platform/opengl/context.h
        vox/src/platform/opengl/renderer_api.cpp
        vox/src/platform/opengl/renderer_api.h
        vox/src/platform/opengl/shader.cpp
        vox/src/platform/opengl/shader.h
        vox/src/platform/opengl/texture.cpp
        vox/src/platform/opengl/texture.h
        vox/src/platform/opengl/vertex_array.cpp
        vox/src/platform/opengl/vertex_array.h

        vox/src/platform/vulkan/vulkan_buffer.cpp
        vox/src/platform/vulkan/vulkan_buffer.h
        vox/src/platform/vulkan/vulkan_renderer_api.cpp
        vox/src/platform/vulkan/vulkan_renderer_api.h
        vox/src/platform/vulkan/vulkan_shader.cpp
        vox/src/platform/vulkan/vulkan_shader.h
        vox/src/platform/vulkan/vulkan_texture.cpp
        vox/src/platform/vulkan/vulkan_texture.h
        vox/src/platform/vulkan/vulkan_vertex_array.cpp
        vox/src/platform/vulkan/vulkan_vertex_array.h
        vox/src/platform/vulkan/vulkan_context.cpp
        vox/src/platform/vulkan/vulkan_context.h
        vox/src/platform/vulkan/vulkan_context_factory.cpp
)

find_package(Vulkan)
include_directories(${Vulkan_INCLUDE_DIRS})

add_library(vox STATIC ${Vox_SOURCES})
target_include_directories(vox PUBLIC ${Vox_DIR})
target_compile_definitions(vox PUBLIC GLFW_INCLUDE_NONE)
target_link_libraries(vox PUBLIC glfw glad glm stb_image ${Vulkan_LIBRARIES})


set(Cube_DIR cube23/src)
set(Cube_SOURCES
        cube23/src/cube_app.cpp
)
add_executable(cube23 ${Cube_SOURCES})
target_include_directories(cube23 PRIVATE ${Vox_DIR})
target_link_libraries(cube23 PRIVATE vox)

# Copy assets for OpenGL app
add_custom_command(TARGET cube23 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/cube23/assets/shaders
    ${CMAKE_CURRENT_BINARY_DIR}/shaders)
add_custom_command(TARGET cube23 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/cube23/assets/textures
    ${CMAKE_CURRENT_BINARY_DIR}/textures)

set(CubeVK_SOURCES
        cube23/src/main.cpp
)
add_executable(cube23_vk ${CubeVK_SOURCES})
target_include_directories(cube23_vk PRIVATE ${Vox_DIR})
target_link_libraries(cube23_vk PRIVATE vox ${Vulkan_LIBRARIES})

include(FetchContent)
FetchContent_Declare(cmake-spirv
        GIT_REPOSITORY https://github.com/liliolett/cmake-spirv.git
        GIT_TAG origin/v1
)
FetchContent_MakeAvailable(cmake-spirv)

list(APPEND CMAKE_MODULE_PATH ${cmake-spirv_SOURCE_DIR}/include)
include(AddSpirvModules)

add_spirv_modules(shaders
        SOURCE_DIR cube23/assets/shaders
        BINARY_DIR shaders
        SOURCES shader.vert shader.frag)

add_dependencies(cube23_vk shaders)

# Copy textures directory to build directory (shaders are already compiled to shaders/ by add_spirv_modules)
add_custom_command(TARGET cube23_vk POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/cube23/assets/textures
    ${CMAKE_CURRENT_BINARY_DIR}/textures)
