cmake_minimum_required(VERSION 3.21)

project(HelloOpenGL VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# custom options to toggle
option(ENABLE_PREPROCESS_OUTPUT "Preprocessed files instead of compiling" OFF) #TODO: doesnt seem to work :)
option(ENABLE_ASM_OUTPUT "Assembly files instead of compiling" OFF)
#------

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(TARGET_PLATFORM "windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(TARGET_PLATFORM "linux")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${TARGET_PLATFORM}/$<CONFIG>")

file(GLOB_RECURSE CPP_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_executable("${CMAKE_PROJECT_NAME}Exec" ${CPP_SOURCES})

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

set_target_properties("${CMAKE_PROJECT_NAME}Exec" PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}") #WARNING! target name here

add_compile_definitions(
        "$<$<CONFIG:Debug>:DEBUG>"
        "$<$<CONFIG:Release>:RELEASE>"
)

# apply options
add_compile_options(
        "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:GNU,Clang>>:-O0;-g;-Wall;-Wextra;-Wpedantic>"
        "$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:GNU,Clang>>:-O2;-Wall;-Wextra>" # O3 risky(try it to check performances) - add flto=thin for Clang(LLVM) and ???? for GCC?
        "$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:/Od;/W4;/Zi>"
        "$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/O2;/W4>"
        "$<$<AND:$<BOOL:${ENABLE_PREPROCESS_OUTPUT}>,$<CXX_COMPILER_ID:GNU,Clang>>:-save-temps=obj>"
        "$<$<AND:$<BOOL:${ENABLE_PREPROCESS_OUTPUT}>,$<CXX_COMPILER_ID:MSVC>>:/P>"
        "$<$<AND:$<BOOL:${ENABLE_ASM_OUTPUT}>,$<CXX_COMPILER_ID:GNU,Clang>>:-save-temps=obj>"
        "$<$<AND:$<BOOL:${ENABLE_ASM_OUTPUT}>,$<CXX_COMPILER_ID:MSVC>>:/FAs>"
)

# msvc specific
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# seen this stuff browsing codebases:
#may give problems on linux ===> then only on windows (and mac?), also check usage
#if(PRODUCTION_BUILD)
#    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization
#else()
#    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
#endif()
# same here, check what it is
#if(MSVC)
#    add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
#endif()
# also this, idk the usage of the first, the 2nd i've seen it somewhere else, 3rd it is already set for CMAKE_samestuff so idk if it's really useful twice
#if(MSVC) # If using the VS compiler...
#
#    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)
#
#    set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console
#
#    #set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreadedDebug<$<CONFIG:Debug>:Debug>")
#    #set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
#
#endif()
# check if really useful
##force remove unicode
#if (WIN32)
#    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /UUNICODE /U_UNICODE)
#endif()

# similar as above, but a bit diff, check it
#if( MSVC )
#    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
#endif()

#and finally this, idk also if rly useful or not for VS
#if( MSVC )
#    if(${CMAKE_VERSION} VERSION_LESS "3.6.0")
#        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n" )
#    else()
#        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLFW-CMake-starter )
#    endif()
#endif()
#-------------------------------------------------------------

# lib options
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) # static lib (used by GLFW, Assimp)

# GLFW options
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
#set(GLFW_BUILD_WIN32 OFF CACHE BOOL "" FORCE) # idc about win32 support => but needed bc fucking win64 still relies on win32 api.

# GLM options
set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)

# Assimp options
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_DOUBLE_PRECISION ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ZLIB ON CACHE BOOL "" FORCE) # fix for "assimp/code/Common/ZipArchiveIOSystem.cpp:57:14: fatal error: unzip.h: No such file or directory" (https://github.com/assimp/assimp/issues/6542)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE) # disabled (sadly) bc of an error in assimp's code (i WONT touch it): "In member function ‘virtual void Assimp::MS3DImporter::InternReadFile(const std::string&, aiScene*, Assimp::IOSystem*)’: assimp/code/AssetLib/MS3D/MS3DLoader.cpp:633:28: error: variable ‘qu’ set but not used [-Werror=unused-but-set-variable=]"

# Libcurl
set(BUILD_CURL_EXE OFF CACHE BOOL "" FORCE)
set(BUILD_LIBCURL_DOCS OFF CACHE BOOL "" FORCE)
set(BUILD_MISC_DOCS OFF CACHE BOOL "" FORCE)
set(ENABLE_CURL_MANUAL OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/glfw)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)
add_subdirectory(vendor/stb)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/assimp)
add_subdirectory(vendor/libcurl)
target_link_libraries("${CMAKE_PROJECT_NAME}Exec" PRIVATE glfw glad glm stb imgui assimp libcurl) #WARNING: target name here