cmake_minimum_required(VERSION 3.26)
project(cube23)

set(CMAKE_CXX_STANDARD 20)

# Add subprojects
add_subdirectory(vox)
add_subdirectory(cube23)

add_subdirectory(vkdemo)
