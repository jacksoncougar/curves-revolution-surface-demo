cmake_minimum_required(VERSION 3.14)
project(gfx_a_iv)

set(CMAKE_CXX_STANDARD 17)


add_executable(gfx_a_iv
include/perlin.h
src/perlin.cpp
include/image.h
src/image.cpp
        src/shading.cpp
        include/shading.h
        src/camera.cpp
        include/camera.h
        src/helpers.cpp
        include/helpers.h
        src/main.cpp
        include/main.h
        src/vec.cpp
        include/vec.h
        src/ray.cpp
        include/ray.h
        src/curve.cpp
        include/curve.h
        src/subdivision.cpp
        include/subdivision.h
        src/transform.cpp
        include/transform.h
        src/matrix.cpp
        include/matrix.h
        src/units.cpp
        include/units.h src/hit.cpp include/hit.h src/sphere.cpp include/sphere.h)

add_library(glad external/glad/src/glad.c)
include_directories(external/glad/include)

add_definitions(-D_USE_MATH_DEFINES -DNOMINMAX  /permissive-)

include_directories(external/stb)
include_directories(include/)

configure_file(shaders/phong.vert.glsl shaders/phong.vert.glsl COPYONLY)
configure_file(shaders/phong.frag.glsl shaders/phong.frag.glsl COPYONLY)

configure_file(shaders/flat.vert.glsl shaders/flat.vert.glsl COPYONLY)
configure_file(shaders/flat.frag.glsl shaders/flat.frag.glsl COPYONLY)

find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME}
        PRIVATE glfw
        PRIVATE glad
        PRIVATE opengl32)


