cmake_minimum_required(VERSION 3.10)
project(HelloWindow)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Generate compile_commands.json for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# === Paths ===
set(GLFW_PATH /Users/connorsullivan/CodeProjects/glfw-3.4.bin.MACOS)
set(GLAD_PATH include)

# === Source files ===
add_executable(main
    src/main.cpp
    src/glad.c
)

# === Include directories ===
include_directories(
    ${GLFW_PATH}/include
    ${GLAD_PATH}
)

# === Link GLFW ===
find_library(GLFW_LIB
    NAMES glfw3 glfw
    PATHS ${GLFW_PATH}/lib-arm64
)

if (NOT GLFW_LIB)
    message(FATAL_ERROR "GLFW library not found in ${GLFW_PATH}/lib-arm64")
endif()

# === Find OpenGL and link macOS frameworks ===
find_package(OpenGL REQUIRED)

target_link_libraries(main
    ${GLFW_LIB}
    ${OPENGL_gl_LIBRARY}
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreFoundation"
    "-framework CoreGraphics"
)

