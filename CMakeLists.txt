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


add_executable(cube23 main.cpp)

target_link_libraries(cube23 ${Vulkan_LIBRARIES} glfw)
