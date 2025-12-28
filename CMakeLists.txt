cmake_minimum_required(VERSION 3.20)
project(OpenGLApp LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GLFW
add_subdirectory(external/glfw)

# Glad
add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

# OpenGL
find_package(OpenGL REQUIRED)

# Renderer
file(GLOB_RECURSE RENDERER_SRC CONFIGURE_DEPENDS src/renderer/*.cpp)
add_library(renderer ${RENDERER_SRC})
target_include_directories(renderer PUBLIC include external/glm external/glad/include)
target_link_libraries(renderer PUBLIC glad OpenGL::GL)

# Engine
file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS src/engine/*.cpp)
add_library(engine ${ENGINE_SRC})
target_include_directories(engine PUBLIC include external/glm external/glad/include)
target_link_libraries(engine PUBLIC renderer glad OpenGL::GL)

# App
add_executable(app src/main.cpp)
target_include_directories(app PRIVATE include external/glm external/glad/include)
target_link_libraries(app
    PRIVATE
        engine
        renderer
        glad
        glfw
        OpenGL::GL
)

# Shaders
add_custom_command(
    TARGET app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/shaders
            $<TARGET_FILE_DIR:app>/shaders
)
