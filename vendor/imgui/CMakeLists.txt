cmake_minimum_required(VERSION 3.12)
project(imgui C)

add_library(imgui)
file(GLOB_RECURSE IMGUI_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(imgui PRIVATE ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(imgui PRIVATE glfw)

#Memo: ImGui's devs recommended to not build it as a static or shared library but rather to directly include the files in the project.
#Source: https://github.com/ocornut/imgui/wiki/Getting-Started#compilinglinking
# However i could not manage to do so in CMake if I wanted to keep the structure for libraries like this: "vendor/lib-name/CMakeLists.txt" added as a subdirectory
# I guess I can do it by either using include() on a .cmake file here which would add sources for the root project
# or by having nothing here and handling the source & include folder in the root CMakeLists.txt

#TODO: check at https://github.com/ocornut/imgui/wiki/Docking and also in the repo "misc/debuggers" and "misc/cpp"