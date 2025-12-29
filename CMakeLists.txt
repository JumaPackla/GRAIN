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
target_include_directories(engine PUBLIC include external/glm)
target_link_libraries(engine PUBLIC glad)

# Simulation
file(GLOB_RECURSE SIMULATION_SRC CONFIGURE_DEPENDS src/simulation/*.cpp)
add_library(simulation ${SIMULATION_SRC})
target_include_directories(simulation PUBLIC include external/glm)
target_link_libraries(simulation PUBLIC engine)

# Forces
file(GLOB_RECURSE FORCES_SRC CONFIGURE_DEPENDS src/forces/*.cpp)
add_library(forces ${FORCES_SRC})
target_include_directories(forces PUBLIC include external/glm)
target_link_libraries(forces PUBLIC engine)

# Particles
file(GLOB_RECURSE PARTICLES_SRC CONFIGURE_DEPENDS src/particles/*.cpp)
add_library(particles ${PARTICLES_SRC})
target_include_directories(particles PUBLIC include external/glm)
target_link_libraries(particles PUBLIC engine renderer)

# App
file(GLOB_RECURSE APP_SRC CONFIGURE_DEPENDS src/app/*.cpp)

add_executable(app
    src/main.cpp
    ${APP_SRC}
)

target_include_directories(app
    PRIVATE
        include
        external/glm
        external/glad/include
)

target_link_libraries(app
    PRIVATE
        engine
        renderer
        simulation
        forces
        particles
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
