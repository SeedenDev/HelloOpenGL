cmake_minimum_required(VERSION 3.12)
project(glad C)

add_library(glad)
target_sources(glad PRIVATE src/glad.c)
target_include_directories(glad PUBLIC include)

#Memo: generated with https://glad.dav1d.de/ using OpenGL 4.5 Core profile and GL_ARB_bindless_texture extension. ("future-proofness" to use newer GL features)