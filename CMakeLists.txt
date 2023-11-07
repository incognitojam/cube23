cmake_minimum_required(VERSION 3.26)
project(cube23)

set(CMAKE_CXX_STANDARD 20)


find_package(Vulkan REQUIRED COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)
include_directories(${Vulkan_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/vox/vendor/glfw)

set(GLM_DIR "../../Libraries/glm")
include_directories(${GLM_DIR})

set(STB_DIR "../../Libraries/stb")
include_directories(${STB_DIR})

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

set(Vox_DIR ${CMAKE_CURRENT_LIST_DIR}/vox/src)
set(Vox_SOURCES
        ${Vox_DIR}/vox/events/event.h
        ${Vox_DIR}/vox/events/key_event.h
        ${Vox_DIR}/vox/events/mouse_event.h
        ${Vox_DIR}/vox/application.cpp
        ${Vox_DIR}/vox/application.h
        ${Vox_DIR}/vox/core.h
        ${Vox_DIR}/vox/entry_point.h
        ${Vox_DIR}/vox/input.cpp
        ${Vox_DIR}/vox/input.h
        ${Vox_DIR}/vox/key_codes.h
        ${Vox_DIR}/vox/mouse_button_codes.h
        ${Vox_DIR}/vox/window.cpp
        ${Vox_DIR}/vox/window.h
        ${Vox_DIR}/vox.h
)

add_library(vox STATIC ${Vox_SOURCES})
target_include_directories(vox PUBLIC ${Vox_DIR})
target_link_libraries(vox ${Vulkan_LIBRARIES} glfw ${OPENGL_LIBRARIES})

set(Cube_DIR ${CMAKE_CURRENT_LIST_DIR}/cube23/src)
set(Cube_SOURCES
        ${Cube_DIR}/cube_app.cpp
)
add_executable(cube23 ${Cube_SOURCES})
target_include_directories(cube23 PUBLIC ${Vox_DIR})
target_link_libraries(cube23 vox ${Vulkan_LIBRARIES} glfw ${OPENGL_LIBRARIES})
