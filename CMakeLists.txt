cmake_minimum_required(VERSION 3.22)
project(RaycastFPS VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(SDL3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(RaycastFPS ${SOURCES})

target_include_directories(RaycastFPS PRIVATE include)
target_link_libraries(RaycastFPS
    SDL3::SDL3
    OpenGL::GL
)
