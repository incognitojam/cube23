# AGENT.md

Graphics programming project with C++20, CMake, OpenGL/Vulkan. Two main apps: `cube23` (OpenGL 3.3+ + Vox engine) and `cube23_vk` (standalone Vulkan).

## Build Commands
```bash
# Configure and build (from project root)
cmake -B cmake-build-debug -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build cmake-build-debug --target cube23 -j 10      # OpenGL app
cmake --build cmake-build-debug --target cube23_vk -j 10    # Vulkan app

# Test/verify build
./test.sh                                                    # Docker-based CI test
```

## Architecture
- **Vox engine** (`vox/src/`): OpenGL 3.3 Core renderer abstraction (Application, Buffer, Shader, Texture, VertexArray)
- **cube23** (`cube23/src/cube_app.cpp`): OpenGL app using Vox engine
- **cube23_vk** (`cube23/src/main.cpp`): Standalone Vulkan implementation
- **Assets**: `cube23/assets/` → auto-copied to build dir, GLSL shaders compiled to SPIR-V

## Code Style
- C++20, `#pragma once`, namespace `Vox` for engine
- Naming: `camelCase` methods, `mMember` variables, `ClassName` types
- Headers: system includes first, then local includes with relative paths
- Smart pointers: `std::shared_ptr`, `std::unique_ptr` with `.reset()` pattern
- GLM for math, GLFW for windowing, platform abstraction in `platform/opengl/`
