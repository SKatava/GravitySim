# GravitySim

**GravitySim** is a simple gravity simulation project written in C++ with dependencies such as **GLFW**, **GLAD**, **ImGui**, and **Implot** for rendering and creating interactive plots.

## Features

- Simulate gravitational forces between objects.
- Use of 2D physics to visualize objects moving under gravitational influence.
- Interactive plotting and visualization of object trajectories.

## Installation

### Requirements:

- C++11 or later
- GLFW, GLAD, ImGui, Implot (included as precompiled binaries in the `lib/` folder)
- CMake (for building the project)

### Steps:

1. Clone the repository:
    
    ```bash
    git clone https://github.com/SKatava/GravitySim.git cd GravitySim
    ```
    
2. Build the project using CMake:
    
    ```bash
    mkdir build cd build cmake .. make
    ```
    
3. Run the simulation:
    ```bash
    .\GravitySim
    ```

## Usage

The simulation will display the gravitational interaction between objects in the system. You can adjust parameters such as mass, velocity, and position to see how they affect the system's behavior over time.

## Dependencies

- GLFW
- GLAD
- ImGui
- Implot

These dependencies are included as precompiled binaries in the `lib/` folder.
