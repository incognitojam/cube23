cmake_minimum_required(VERSION 3.26)
project(vox)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/stb)

set(Vox_DIR src)
set(Vox_SOURCES
        src/vox/core/timestep.h
        src/vox/events/event.h
        src/vox/events/key_event.h
        src/vox/events/mouse_event.h
        src/vox/renderer/buffer.cpp
        src/vox/renderer/buffer.h
        src/vox/renderer/graphics_context.h
        src/vox/renderer/orthographic_camera.cpp
        src/vox/renderer/orthographic_camera.h
        src/vox/renderer/render_command.cpp
        src/vox/renderer/render_command.h
        src/vox/renderer/renderer.cpp
        src/vox/renderer/renderer.h
        src/vox/renderer/renderer_api.h
        src/vox/renderer/shader.cpp
        src/vox/renderer/shader.h
        src/vox/renderer/texture.cpp
        src/vox/renderer/texture.h
        src/vox/renderer/vertex_array.cpp
        src/vox/renderer/vertex_array.h
        src/vox/application.cpp
        src/vox/application.h
        src/vox/core.h
        src/vox/entry_point.h
        src/vox/input.h
        src/vox/key_codes.h
        src/vox/mouse_button_codes.h
        src/vox/window.cpp
        src/vox/window.h
        src/Vox.h

        src/platform/opengl/buffer.cpp
        src/platform/opengl/buffer.h
        src/platform/opengl/context.cpp
        src/platform/opengl/context.h
        src/platform/opengl/renderer_api.cpp
        src/platform/opengl/renderer_api.h
        src/platform/opengl/shader.cpp
        src/platform/opengl/shader.h
        src/platform/opengl/texture.cpp
        src/platform/opengl/texture.h
        src/platform/opengl/vertex_array.cpp
        src/platform/opengl/vertex_array.h

        src/platform/vulkan/vulkan_buffer.cpp
        src/platform/vulkan/vulkan_buffer.h
        src/platform/vulkan/vulkan_renderer_api.cpp
        src/platform/vulkan/vulkan_renderer_api.h
        src/platform/vulkan/vulkan_shader.cpp
        src/platform/vulkan/vulkan_shader.h
        src/platform/vulkan/vulkan_texture.cpp
        src/platform/vulkan/vulkan_texture.h
        src/platform/vulkan/vulkan_vertex_array.cpp
        src/platform/vulkan/vulkan_vertex_array.h
        src/platform/vulkan/vulkan_context.cpp
        src/platform/vulkan/vulkan_context.h
        src/platform/vulkan/vulkan_context_factory.cpp
)

find_package(Vulkan)
include_directories(${Vulkan_INCLUDE_DIRS})

add_library(vox STATIC ${Vox_SOURCES})
target_include_directories(vox PUBLIC ${Vox_DIR})
target_compile_definitions(vox PUBLIC GLFW_INCLUDE_NONE)
target_link_libraries(vox PUBLIC glfw glad glm stb_image ${Vulkan_LIBRARIES})
