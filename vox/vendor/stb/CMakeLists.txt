cmake_minimum_required(VERSION 3.19)
project(stb)

add_library(stb INTERFACE include/stb_image.h)
target_include_directories(stb INTERFACE include)
