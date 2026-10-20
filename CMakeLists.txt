cmake_minimum_required(VERSION 3.20)

project(OpenGLTutorial)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
glfw
GIT_REPOSITORY https://github.com/glfw/glfw.git
GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glm)


find_package(OpenGL REQUIRED)
find_package(Freetype REQUIRED)

add_library(glad STATIC
external/glad/src/gl.c
)

target_include_directories(glad PUBLIC
external/glad/include
)

add_executable(OpenGLTutorial
    src/main.cpp
    src/Card.cpp
    src/CardRenderer.cpp
    src/TextureLibrary.cpp
    src/FieldRenderer.cpp
    src/Field.cpp
    src/Hand.cpp
    src/TextRenderer.cpp
    src/PhaseRenderer.cpp
)

target_include_directories(OpenGLTutorial PRIVATE
    include
)

target_link_libraries(OpenGLTutorial PRIVATE
glfw
glad
OpenGL::GL
glm::glm
Freetype::Freetype
)