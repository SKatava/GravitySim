cmake_minimum_required(VERSION 3.31.1)

set( CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe" )
set( CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe" )

project(GravitySim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXE_LINKER_FLAGS "-static")


configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/default.vert ${CMAKE_BINARY_DIR}/assets/shaders/default.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/default.frag ${CMAKE_BINARY_DIR}/assets/shaders/default.frag COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/particle.vert ${CMAKE_BINARY_DIR}/assets/shaders/particle.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/particle.frag ${CMAKE_BINARY_DIR}/assets/shaders/particle.frag COPYONLY)


configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/particleSSBO.vert ${CMAKE_BINARY_DIR}/assets/shaders/particleSSBO.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/particleSSBO.frag ${CMAKE_BINARY_DIR}/assets/shaders/particleSSBO.frag COPYONLY)


configure_file(${CMAKE_SOURCE_DIR}/assets/shaders/gravityDirectMethod.comp ${CMAKE_BINARY_DIR}/assets/shaders/gravityDirectMethod.comp COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/assets/fonts/icon_font.otf ${CMAKE_BINARY_DIR}/assets/fonts/icon_font.otf COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/assets/fonts/inter.ttf ${CMAKE_BINARY_DIR}/assets/fonts/inter.ttf COPYONLY)

configure_file(${CMAKE_SOURCE_DIR}/assets/icons/icon.png ${CMAKE_BINARY_DIR}/assets/icons/icon.png COPYONLY)

SET(GLAD_DIR "lib/glad")
set(GLFW_DIR "lib/glfw")

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/lib/imgui) 
set(IMGUI_SOURCES 
    ${IMGUI_DIR}/imgui.cpp 
    ${IMGUI_DIR}/imgui_demo.cpp 
    ${IMGUI_DIR}/imgui_draw.cpp 
    ${IMGUI_DIR}/imgui_widgets.cpp 
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp 
)

set(IMPLOT_DIR ${CMAKE_SOURCE_DIR}/lib/implot)
set(IMPLOT_SOURCES
    ${IMPLOT_DIR}/implot.cpp
    ${IMPLOT_DIR}/implot_items.cpp
)

set(STB_DIR ${CMAKE_SOURCE_DIR}/lib/stb)

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

set(SOURCES
    src/main.cpp

    src/Core/Application.cpp
    src/Core/Timer.cpp
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

    lib/glad/glad.c
)

add_executable(GravitySim WIN32 ${SOURCES} ${IMGUI_SOURCES} ${IMPLOT_SOURCES} assets/icons/resource.rc)

add_subdirectory(${GLFW_DIR})
target_link_libraries(GravitySim PRIVATE glfw)

add_library(glad STATIC ${GLAD_DIR}/glad.c)
target_include_directories(glad PUBLIC ${GLAD_DIR}/include)
target_link_libraries(GravitySim PRIVATE glad)

find_package(OpenGL REQUIRED)
target_link_libraries(GravitySim PRIVATE OpenGL::GL)

target_link_libraries(GravitySim PRIVATE glfw dwmapi)


target_include_directories(${PROJECT_NAME} PRIVATE 
    ${GLFW_DIR}/include 
    ${GLAD_DIR}/include 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${IMPLOT_DIR}
    ${STB_DIR}
    include/
)

