cmake_minimum_required(VERSION 3.26)
project(cube23)

set(CMAKE_CXX_STANDARD 20)


find_package(Vulkan REQUIRED COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

set(GLFW_DIR "../../Libraries/glfw-3.3.8")
add_subdirectory(${GLFW_DIR} glfw)

include_directories(${Vulkan_INCLUDE_DIRS} ${GLFW_DIR}/include)

set(GLM_DIR "../../Libraries/glm")
include_directories(${GLM_DIR})

set(STB_DIR "../../Libraries/stb")
include_directories(${STB_DIR})


set(Vox_DIR ${CMAKE_CURRENT_LIST_DIR}/vox/src)
set(Vox_SOURCES
        ${Vox_DIR}/Vox/Application.h
        ${Vox_DIR}/Vox/Application.cpp
        ${Vox_DIR}/Vox/EntryPoint.h
        ${Vox_DIR}/Vox.h
)

add_library(vox STATIC ${Vox_SOURCES})
target_include_directories(vox PUBLIC ${Vox_DIR})
target_link_libraries(vox ${Vulkan_LIBRARIES} glfw)

set(Cube_DIR ${CMAKE_CURRENT_LIST_DIR}/cube23/src)
set(Cube_SOURCES
        ${Cube_DIR}/CubeApp.cpp
)
add_executable(cube23 ${Cube_SOURCES})
target_include_directories(cube23 PUBLIC ${Vox_DIR})
target_link_libraries(cube23 vox ${Vulkan_LIBRARIES} glfw)
