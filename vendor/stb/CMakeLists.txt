cmake_minimum_required(VERSION 3.21)
project(stb C)

add_library(stb)
target_sources(stb PRIVATE stb_image.c)
target_include_directories(stb PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

#Memo: i would have preferred including files directly into the root project but my CMake skillset does not allow me to do so (see imgui/CMakeLists.txt for more explanation)