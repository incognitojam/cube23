cmake_minimum_required(VERSION 3.26)
project(vox)

set(CMAKE_CXX_STANDARD 20)

set(GLFW_BUILD_WAYLAND OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/stb)

# Add spirv-cross for shader reflection
include(FetchContent)
FetchContent_Declare(spirv-cross
    GIT_REPOSITORY https://github.com/KhronosGroup/SPIRV-Cross.git
    GIT_TAG main
)
FetchContent_MakeAvailable(spirv-cross)

# Add cmake-spirv for shader compilation
FetchContent_Declare(cmake-spirv
    GIT_REPOSITORY https://github.com/liliolett/cmake-spirv.git
    GIT_TAG origin/v1
)
FetchContent_MakeAvailable(cmake-spirv)

list(APPEND CMAKE_MODULE_PATH ${cmake-spirv_SOURCE_DIR}/include)
include(AddSpirvModules)

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
)

add_library(vox STATIC ${Vox_SOURCES})
target_include_directories(vox PUBLIC ${Vox_DIR})
target_compile_definitions(vox PUBLIC GLFW_INCLUDE_NONE)
target_link_libraries(vox PUBLIC glfw glad glm stb_image spirv-cross-core spirv-cross-glsl)
