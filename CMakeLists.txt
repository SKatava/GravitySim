cmake_minimum_required(VERSION 3.10)
project(GravitySim)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -g")

cmake_policy(SET CMP0072 NEW)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)


# Find Packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)


configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/default.vert ${CMAKE_BINARY_DIR}/shaders/default.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/default.frag ${CMAKE_BINARY_DIR}/shaders/default.frag COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/particle.vert ${CMAKE_BINARY_DIR}/shaders/particle.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/particle.frag ${CMAKE_BINARY_DIR}/shaders/particle.frag COPYONLY)


configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/particleSSBO.vert ${CMAKE_BINARY_DIR}/shaders/particleSSBO.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/particleSSBO.frag ${CMAKE_BINARY_DIR}/shaders/particleSSBO.frag COPYONLY)


configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/gravityDirectMethod.comp ${CMAKE_BINARY_DIR}/shaders/gravityDirectMethod.comp COPYONLY)

# Include Directories
include_directories(
    include/Compute
    include/Core
    include/Rendering
    include/GUI
    include/Entities
    include/Events
    include/Physics
    include/DataTypes
    lib/glad/include
    lib/imgui
    lib/imgui/backends
    lib/implot
)

# Source Files
set(SOURCES
    src/main.cpp

    src/Core/Application.cpp
    src/Core/Time.cpp
    src/Core/Settings.cpp
    src/Core/Hardware.cpp

    src/Rendering/Renderer.cpp    
    src/Rendering/VAO.cpp    
    src/Rendering/VBO.cpp    
    src/Rendering/Circle.cpp
    src/Rendering/Shader.cpp
    src/Rendering/Line.cpp
    src/Rendering/Background.cpp
    src/Rendering/Dots.cpp

    src/GUI/GUI.cpp
    src/GUI/Positions.cpp

    src/Entities/Entity.cpp
    src/Entities/Entities.cpp

    src/Events/Events.cpp

    src/Physics/GravityEngine.cpp
    src/Physics/PhysicsEngine.cpp

    src/Compute/SSBO.cpp
    src/Compute/ComputeShader.cpp
    src/Compute/ComputeSystem.cpp

    src/DataTypes/Vector2f.cpp
    src/DataTypes/Vector3f.cpp
    src/DataTypes/Vector2d.cpp
    src/DataTypes/BHTree.cpp

    lib/glad/glad.c

    lib/imgui/imgui.cpp
    lib/imgui/imgui_draw.cpp
    lib/imgui/imgui_tables.cpp
    lib/imgui/imgui_widgets.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp 
    lib/imgui/backends/imgui_impl_opengl3.cpp

    lib/implot/implot.cpp
    lib/implot/implot_items.cpp
    
)

# Build Executable
add_executable(GravitySim ${SOURCES})

# Link Libraries
target_link_libraries(GravitySim PRIVATE OpenGL::GL glfw)

