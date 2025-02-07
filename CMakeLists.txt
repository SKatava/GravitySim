cmake_minimum_required(VERSION 3.31.1)

set( CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe" )
set( CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe" )

project(GravitySim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXE_LINKER_FLAGS "-static")

configure_file(${CMAKE_SOURCE_DIR}/shaders/default.vert ${CMAKE_BINARY_DIR}/shaders/default.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/shaders/default.frag ${CMAKE_BINARY_DIR}/shaders/default.frag COPYONLY)

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

set(SOURCES
    ${GLAD_DIR}/glad.c
    src/main.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/Vector2f.cpp
    src/shaderClass.cpp
    src/Vector3f.cpp
    src/Dot.cpp
    src/Line.cpp
    src/Window.cpp
    src/Circle.cpp
    src/Object.cpp
    src/globals.cpp
    src/ObjectManager.cpp
    src/Background.cpp
    src/GravityEngine.cpp
    src/CollisionSolution.cpp
    src/GUI.cpp
)

add_executable(GravitySim WIN32 ${SOURCES} ${IMGUI_SOURCES} ${IMPLOT_SOURCES} resource.rc)

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