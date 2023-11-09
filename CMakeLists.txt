cmake_minimum_required(VERSION 3.26)
project(cube23)

set(CMAKE_CXX_STANDARD 20)


find_package(Vulkan REQUIRED COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)
include_directories(${Vulkan_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vox/vendor/glfw)

add_subdirectory(vox/vendor/glad)

add_subdirectory(vox/vendor/glm)

add_subdirectory(vox/vendor/stb)

set(Vox_DIR vox/src)
set(Vox_SOURCES
        vox/src/vox/events/event.h
        vox/src/vox/events/key_event.h
        vox/src/vox/events/mouse_event.h
        vox/src/vox/renderer/buffer.cpp
        vox/src/vox/renderer/buffer.h
        vox/src/vox/renderer/graphics_context.h
        vox/src/vox/renderer/render_command.cpp
        vox/src/vox/renderer/render_command.h
        vox/src/vox/renderer/renderer.cpp
        vox/src/vox/renderer/renderer.h
        vox/src/vox/renderer/renderer_api.cpp
        vox/src/vox/renderer/renderer_api.h
        vox/src/vox/renderer/shader.cpp
        vox/src/vox/renderer/shader.h
        vox/src/vox/renderer/vertex_array.cpp
        vox/src/vox/renderer/vertex_array.h
        vox/src/vox/application.cpp
        vox/src/vox/application.h
        vox/src/vox/core.h
        vox/src/vox/entry_point.h
        vox/src/vox/input.cpp
        vox/src/vox/input.h
        vox/src/vox/key_codes.h
        vox/src/vox/mouse_button_codes.h
        vox/src/vox/window.cpp
        vox/src/vox/window.h
        vox/src/vox.h

        vox/src/platform/opengl/buffer.cpp
        vox/src/platform/opengl/buffer.h
        vox/src/platform/opengl/context.cpp
        vox/src/platform/opengl/context.h
        vox/src/platform/opengl/renderer_api.cpp
        vox/src/platform/opengl/renderer_api.h
        vox/src/platform/opengl/vertex_array.cpp
        vox/src/platform/opengl/vertex_array.h
)

add_library(vox STATIC ${Vox_SOURCES})
target_include_directories(vox PUBLIC ${Vox_DIR})
target_compile_definitions(vox PUBLIC GLFW_INCLUDE_NONE)
target_link_libraries(vox PUBLIC ${Vulkan_LIBRARIES} glfw glad glm stb)

set(Cube_DIR cube23/src)
set(Cube_SOURCES
        cube23/src/cube_app.cpp
)
add_executable(cube23 ${Cube_SOURCES})
target_include_directories(cube23 PRIVATE ${Vox_DIR})
target_link_libraries(cube23 PRIVATE vox)
