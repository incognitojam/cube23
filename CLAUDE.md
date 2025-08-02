# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a graphics programming project with two main components:
1. **Vox** - A custom game engine/renderer framework with OpenGL support
2. **Cube23** - Applications built on top of Vox, including both OpenGL (`cube23`) and Vulkan (`cube23_vk`) implementations

## Build System

The project uses CMake with C++20 standard. Two executables are built:

- `cube23` - OpenGL-based application using the Vox engine
- `cube23_vk` - Standalone Vulkan application with comprehensive 3D rendering

### Build Commands

```bash
# Configure and build from cmake-build-debug directory
cmake --build /path/to/cmake-build-debug --target cube23 -j 10
cmake --build /path/to/cmake-build-debug --target cube23_vk -j 10

# Or build specific targets
cmake --build cmake-build-debug --target cube23
cmake --build cmake-build-debug --target cube23_vk
```

### Shader Compilation

The Vulkan application automatically compiles GLSL shaders to SPIR-V:
- Source shaders: `cube23/assets/shaders/*.{vert,frag}`
- Compiled output: `cmake-build-debug/shaders/*.spv`
- Uses cmake-spirv extension with `add_spirv_modules`

### Asset Management

Assets are automatically copied during build:
- Textures from `cube23/assets/textures/` → `cmake-build-debug/textures/`
- Compiled shaders go to `cmake-build-debug/shaders/`

## Architecture

### Vox Engine (vox/src/)

The Vox engine follows a platform abstraction pattern:

**Core Architecture:**
- `vox/application.h` - Main application base class with game loop
- `vox/entry_point.h` - Application entry point macro
- `vox/renderer/` - Rendering abstraction layer
- `platform/opengl/` - OpenGL-specific implementations

**Renderer Abstraction:**
- `RendererAPI` - Graphics API abstraction (currently OpenGL only)
- `Buffer` - Vertex/Index buffer management with layout system
- `Shader` - Shader loading and uniform management
- `Texture` - Texture loading and binding
- `VertexArray` - Vertex attribute binding
- `Renderer` - High-level rendering commands

**Application Pattern:**
Applications inherit from `Vox::Application` and implement:
- `onUpdate(Timestep)` - Per-frame logic
- Constructor sets up resources (buffers, shaders, textures)
- `Vox::create_application()` factory function

### Cube23 Applications

**cube23 (OpenGL):** Demonstrates the Vox engine with:
- 2D orthographic camera with keyboard controls
- Textured quad rendering in a grid pattern
- Shader library management
- Multiple texture switching

**cube23_vk (Vulkan):** Standalone Vulkan implementation featuring:
- Complete Vulkan initialization with validation layers
- 3D perspective camera with model-view-projection matrices
- Cube geometry with vertex colors and texture coordinates
- Depth testing and depth buffer
- Uniform buffer objects with descriptor sets
- Image layout transitions and memory management
- Swapchain management with recreation support
- Frame synchronization with semaphores and fences

### Key Dependencies

- **GLFW** - Window management and input (both OpenGL and Vulkan)
- **GLM** - Mathematics library
- **GLAD** - OpenGL loader (Vox engine only)
- **STB** - Image loading for textures
- **Vulkan SDK** - Vulkan API (cube23_vk only)

## Development Notes

### Adding New Vox Applications

1. Create source file inheriting from `Vox::Application`
2. Implement constructor for resource setup
3. Override `onUpdate(Timestep)` for frame logic
4. Implement `Vox::create_application()` factory
5. Add to CMakeLists.txt with vox library dependency

### Shader Development

**OpenGL (Vox):** GLSL source files loaded at runtime from `assets/shaders/`

**Vulkan:** GLSL source files compiled to SPIR-V at build time:
- Edit: `cube23/assets/shaders/*.{vert,frag}`
- Build automatically compiles to `cmake-build-debug/shaders/*.spv`
- Applications load `.spv` files using `readFile()` function

### Graphics API Support

Current status:
- **OpenGL**: Fully supported through Vox engine abstraction
- **Vulkan**: Complete standalone implementation in cube23_vk
- **Future**: Vulkan backend could be integrated into Vox's platform abstraction

The cube23_vk application serves as a reference implementation for Vulkan features that could potentially be integrated into the Vox engine's renderer abstraction layer.

### Asset Pipeline

Assets should be placed in `cube23/assets/` and will be automatically copied to the build directory:
- Textures: `.jpg`, `.png` files
- Shaders: `.vert`, `.frag` GLSL source files
- Build system handles copying and shader compilation transparently

### Documentation and Notes

The `notes/` directory contains project documentation and analysis:
- Technical analyses and architecture decisions
- Implementation notes for future development
- Research findings and feasibility studies
- Use this directory for documenting complex changes or architectural decisions