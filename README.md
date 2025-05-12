# GravitySim 2.0

<p align="center">
  <img src="img%20showcase/icon.png" width="300" height="300">
</p>

## Introduction

GravitySim is an desktop application designed to calculate and render effects of gravitational force using the following formula at it's core:

$$
F = G \frac{m_1 m_2}{r^2}
$$

<p align="center">
  <img src="img%20showcase/example_one.png" ">
</p>
<p align="center">
  <img src="img%20showcase/example_two.png" >
</p>
<p align="center">
  <img src="img%20showcase/example_three.png">
</p>

The algorithm for calucating this equation is normaly of $O(n^2)$ complexity, but in this program, when calculating force, entities are not treated as individuals, but rather as conections, that is possible because the formula returns the same value on both ends, so two entities can be done at once, with that complexity drops to $O(\frac{n^2}{2})$, and when the entities that are already done with calculaion are excluded we get the final complexity of $O(\frac{n(n-1)}{2})$.

With that said, just that can only handle around 1000 objects at resonable FPS on decent computers. Two increase the power there are two more options(High and Ultra).

### High Perfomance Mode

To fasten up the computer to do more calclulation quite a bit I added a multithreading to the physics calculations, and with that my pc with 8 cores and 16 threads can run about 5000 objects at resonable FPS with much higher CPU usage cost.

### Ultra Perfomance Mode

The last step of imporving the speed and perfomance of simulation was to paralize the proccess even more, and to do so I added GLSL shader for calculation of gravity force as well as updating the entities positions. GLSL shader is a script writen in similar syntax as c++ that gives direct orders to GPU, and that is great because GPU often has hundreds, even thousands of cores that can calculate in paralel. With that my PC was able to get 50 000 objects at around 15-20 FPS, which is good considering GPU had has around 780 cores, while there are many much stronger GPU's that can realy show some high perfomance. 

### UI

For the UI the best option was to use ImGui, it is fast and minimal, making it perfect for implementing in high-perfomance project. The UI has the following features:
- Showing data analytics
- Add entity form
- Select entity form(for group of entities)
- Delete options
- Simple time manipulation options
- CPU data showcase

### Technologies
The following technologies were used for making this application:
- C++
- CMake
- OpenGL
- GLSL
- GLFW
- Glad
- ImGui
- ImPlot

# Get Started

### Instalation on Windows(10,11) 
If you are just a normal user wanting to use and test the application, there is a installer in release section that will donwload the application to your desktop.

### Building on Windows(10, 11)
If you want to work on the project yourself and build it you already have the CMakeLists.txt file, what you do need is to setup dependecies in a "lib/" folder, the dependecies and the structure is the following:
```
/lib/
  |-glfw
  |-glad
  |-imgui
  |-implot
  |-stb
```
After making sure all dependecies are setup, you can build the project in "/build/" directory, and to do so you need CMake and c++ compiler. Example with MinGW gcc compiler:
```
cmake .. -G "MinGW Makefiles"
```
And after successfuly creating cmake build direcotry you can build with the following command:
```
cmake --build .
```
### Linux installation
I didn't make a good way of installing it on Linux even tho this application was originaly bult on Linux...





