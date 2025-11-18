# Hierarchical Walking Animation System

A modern OpenGL-based hierarchical motion control system implemented in C++ for computer animation coursework. Features articulated figure animation with synchronized walking motion along spline-defined paths.

## Table of Contents

- [Description](#description)
- [Features](#features)
- [Requirements](#requirements)
- [How to Build](#how-to-build)
  - [Linux](#linux)
  - [Windows](#windows)
- [How to Use](#how-to-use)
  - [Command Line Options](#command-line-options)
  - [Interactive Controls](#interactive-controls)
- [Control Points File Format](#control-points-file-format)
- [Examples](#examples)
- [Project Structure](#project-structure)
- [Technical Details](#technical-details)
- [Performance Notes](#performance-notes)

## Description

This project implements a hierarchical articulated figure animation system using modern OpenGL with GLFW. It demonstrates fundamental computer animation concepts including:

- **Forward kinematics** with hierarchical transformations
- **Spline-based path following** (Catmull-Rom and B-spline)
- **Synchronized walking animation** (no moon-walking!)
- **Velocity-adaptive motion** with automatic gait synchronization
- **Interactive 3D visualization** with camera controls

The system animates a figure consisting of a torso and two articulated legs (each with hip and knee joints) walking along a user-defined spline path.

## Features

### Core Animation
- **Hierarchical Structure**: Proper parent-child joint relationships using OpenGL matrix stack
- **Multiple Spline Types**: Catmull-Rom and B-spline interpolation for path definition
- **Synchronized Walking**: Leg motion automatically synchronized with movement speed
- **Velocity-Adaptive Gait**: Walk cycle frequency matches traversal velocity (prevents moon-walking)

### Advanced Features
- **Body Leaning**: Torso tilts forward based on movement speed
- **Proper Orientation**: Figure faces direction of motion at all times
- **2-DOF Leg Articulation**: Independent hip swing and knee bending per leg
- **Phase-Opposite Legs**: Left and right legs move 180° out of phase
- **Smooth Closed Loops**: Seamless animation when path returns to start

### Rendering & Interaction
- **Real-time 3D Rendering**: OpenGL with GLU primitives (cylinders, boxes, spheres)
- **Interactive Camera**: Mouse-controlled rotation, zoom, and orbit
- **Path Visualization**: Display of spline curve and control points
- **Runtime Speed Control**: Adjust animation and walk speed on-the-fly
- **Cross-platform**: Supports Linux, and Windows

## Requirements

### Build Dependencies
- **CMake** (3.10+) or **GNU Make**
- **C++ Compiler** supporting C++11 (GCC 4.8+, Clang 3.3+, MSVC 2015+)

### Runtime Libraries
- **OpenGL** 2.1+ (with GLU)
- **GLFW3** (window management and input)
- **GLEW** (OpenGL extension loading)
- **GLM** (mathematics library, optional)

### Platform-Specific
- **Linux**: X11 development libraries
- **Windows**: Visual Studio 2015+ or MinGW-w64

## How to Build

### Linux

**Install dependencies:**
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install libglfw3-dev libglew-dev libglu1-mesa-dev

# Fedora
sudo dnf install cmake gcc-c++ pkgconfig
sudo dnf install glfw-devel glew-devel mesa-libGLU-devel
```

**Build the project:**
```bash
# Using CMake
mkdir build
cmake --preset linux-vcpkg
cmake --build build --config Release
./hierarchical_walking_animation ../control_points.txt
```

### Windows

**Using vcpkg (Recommended):**
```bash
# Build with CMake
mkdir build
cmake --preset win-vcpkg
cmake --build build --config Release
```

**Using Visual Studio:**
1. Open CMake project in Visual Studio
2. CMake will automatically detect vcpkg packages
3. Build → Build Solution
4. Run from `build/Release/hierarchical_walking_animation.exe`


## How to Use

### Command Line Options

```bash
./hierarchical_walking_animation [control_points_file]
```

**Arguments:**
- `control_points_file` - Path to control points file (default: `control_points.txt`)

**Examples:**
```bash
# Use custom path file
./hierarchical_walking_animation my_path.txt

```

### Interactive Controls

#### Keyboard Controls
| Key | Action |
|-----|--------|
| **+** | Increase overall animation speed |
| **-** | Decrease overall animation speed |
| **R** | Reset animation to beginning |
| **ESC** | Exit application |

#### Mouse Controls
| Action | Result |
|--------|--------|
| **Left Click + Drag** | Rotate camera around figure |
| **Mouse Wheel** | Zoom in/out |
| **Window Resize** | Automatically adjusts viewport |

## Control Points File Format

Control points files define the spline path for the figure to follow.

### File Structure

```
<SPLINE_TYPE>
<dt_value>
<x1> <y1> <z1>
<x2> <y2> <z2>
...
```

### Parameters

**Line 1: Spline Type**
- `CATMULL_ROM` - Catmull-Rom spline (passes through control points)
- `BSPLINE` - B-spline (smooth approximation)

**Line 2: Time Step (dt)**
- Controls animation speed (0.0 to 1.0+)
- Smaller values = slower animation
- Larger values = faster animation
- Recommended range: 0.01 to 0.5

**Remaining Lines: Control Points**
- Each line: `x y z` (space-separated)
- Coordinates in world space
- Minimum 4 points required for splines
- For closed loops, duplicate first 2-3 points at end

### Example Files

**Simple Path (control_points.txt):**
```
CATMULL_ROM
0.1
-2.0 0.0 1.0
0.0 0.0 0.0
2.0 0.0 1.0
4.0 0.0 3.0
5.0 0.0 5.0
```

**Closed Loop:**
```
CATMULL_ROM
0.1
0.0 0.0 0.0
3.0 0.0 0.0
3.0 0.0 3.0
0.0 0.0 3.0
-3.0 0.0 3.0
-3.0 0.0 0.0
0.0 0.0 0.0
3.0 0.0 0.0
```

**B-Spline (control_points_bspline.txt):**
```
BSPLINE
0.05
0.0 0.0 0.0
3.0 0.0 0.0
3.0 0.0 3.0
0.0 0.0 3.0
-3.0 0.0 3.0
-3.0 0.0 0.0
0.0 0.0 0.0
```

## Examples

### Basic Usage

**Run:**
```bash
./hierarchical_walking_animation control_points.txt
```

### Creating Custom Paths

**Straight line:**
```
CATMULL_ROM
0.1
-5.0 0.0 0.0
-2.0 0.0 0.0
2.0 0.0 0.0
5.0 0.0 0.0
```

**Circle (8 points):**
```
CATMULL_ROM
0.1
3.0 0.0 0.0
2.1 0.0 2.1
0.0 0.0 3.0
-2.1 0.0 2.1
-3.0 0.0 0.0
-2.1 0.0 -2.1
0.0 0.0 -3.0
2.1 0.0 -2.1
3.0 0.0 0.0
2.1 0.0 2.1
```

**Figure-8:**
```
BSPLINE
0.08
0.0 0.0 0.0
3.0 0.0 2.0
0.0 0.0 4.0
-3.0 0.0 2.0
0.0 0.0 0.0
3.0 0.0 -2.0
0.0 0.0 -4.0
-3.0 0.0 -2.0
0.0 0.0 0.0
```

### Speed Adjustment

**Very slow walk:**
- Set `dt = 0.01` in control points file
- Or press `-` multiple times during runtime

**Fast walk:**
- Set `dt = 0.5` in control points file
- Or press `+` multiple times during runtime

## Project Structure


### Module Overview

| Module | Purpose |
|--------|---------|
| **Constants** | Global constants (dimensions, PI) |
| **Vec3** | 3D vector operations |
| **ArticulatedFigure** | Figure state (position, joint angles) |
| **Spline** | Catmull-Rom and B-spline evaluation |
| **Renderer** | OpenGL drawing (primitives, figure, scene) |
| **Animation** | Walking animation update logic |
| **FileIO** | Control points file parsing |
| **main** | GLFW setup, callbacks, main loop |

## Technical Details

### Spline Evaluation

**Catmull-Rom Spline:**
- Interpolates through control points
- C¹ continuous
- Local control (4-point segments)
- Better for precise path following

**B-Spline:**
- Approximates control points
- C² continuous
- Smooth curve
- Better for organic motion

**Tangent Calculation:**
```cpp
forward = normalize(position(t + ε) - position(t))
```

### Velocity-Based Gait

The system prevents "moon-walking" by synchronizing leg movement with actual traversal speed:

```cpp
velocity = newPosition - currentPosition
speed = |velocity|
walkCycle += speed × walkSpeed × multiplier
```

This ensures:
- Faster movement = faster leg motion
- Slower movement = slower leg motion
- Standing still = no leg motion

## Performance Notes

The system is optimized for smooth real-time animation:

### Optimizations
- **Efficient spline evaluation** with minimal allocations
- **Direct GLU primitives** (no mesh generation overhead)
- **Minimal state changes** in rendering loop
- **Simple collision-free animation** (no physics calculations)

### Typical Performance
- **60 FPS** on modern integrated GPUs
- **Rendering time**: < 1ms per frame
- **Animation update**: < 0.1ms per frame
- **Memory usage**: < 10MB
