cmake_minimum_required(VERSION 3.5)
# Project Name
PROJECT(LightingMap)

SET(Study_OpenGL_INCLUDE_DIRS "../OpenGL")
SET(Study_OpenGL_LIBRARY_DIRS "../OpenGL/lib")
include_directories(${Study_OpenGL_INCLUDE_DIRS}/include)
link_directories(${Study_OpenGL_LIBRARY_DIRS})

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

add_executable(LightingMap main.cpp)
target_link_libraries(LightingMap glew32d.lib libglew32d.lib glfw3.lib ${OPENGL_gl_LIBRARY})

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})