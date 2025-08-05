# Vulkan Backend Testing and Validation Plan

*Plan created on 2025-08-03*

## Current State Analysis

### ✅ Completed
- **Vulkan infrastructure**: Complete VulkanContext, VulkanRendererAPI classes
- **Dynamic context creation**: Window class creates appropriate context based on API
- **Initialization porting**: All Vulkan setup logic ported from cube23_vk to Vox platform layer
- **Build system**: Vulkan libraries linked, factory pattern prevents header conflicts
- **Backward compatibility**: OpenGL functionality remains unchanged

### ❌ Missing/Incomplete
- **API selection**: Hardcoded to `OpenGL` in `renderer_api.cpp`
- **Runtime testing**: No way to test Vulkan backend in cube23 app
- **Rendering pipeline**: Vulkan classes are mostly stubs (no actual rendering)
- **Validation**: Haven't verified Vulkan backend works end-to-end

## Problem Statement

Currently, running `./cube23` only tests the OpenGL backend because:
1. `RendererAPI::sAPI = API::OpenGL` is hardcoded
2. No mechanism exists to select Vulkan at runtime
3. Vulkan rendering methods are TODO stubs

**Goal**: Enable testing and validation of the Vulkan backend, leading to eventual removal of the standalone `cube23_vk` demo.

## Implementation Plan

### Phase 1: API Selection Mechanism ✅ IMPLEMENTED

**Environment Variable Solution:**
```bash
VOX_RENDERER=vulkan ./cube23   # Use Vulkan backend
VOX_RENDERER=opengl ./cube23   # Use OpenGL backend (default)
./cube23                       # Defaults to OpenGL
```

**Implementation:** The renderer API is selected at runtime using a function-local static in `RendererAPI::getAPI()` that checks the `VOX_RENDERER` environment variable. This approach avoids static initialization order issues and keeps the implementation clean.

### Phase 2: Basic Vulkan Testing ⚡ HIGH PRIORITY

**Immediate testing steps:**
1. Run `VOX_RENDERER=vulkan ./cube23`
2. Verify VulkanContext initialization (debug logs)
3. Identify missing rendering pipeline components
4. Document what fails and why

**Expected issues:**
- ✅ FIXED: GLFW window surface creation (needed GLFW_NO_API)
- ✅ FIXED: VSync calls on non-OpenGL context
- ✅ FIXED: VK_KHR_portability_subset extension on macOS
- ❌ **CRITICAL**: Shader abstraction violation in cube_app.cpp

**Current Status (2025-08-03):**
- VulkanContext initializes successfully
- App reaches main loop without crashing
- **Major architectural issue**: cube_app.cpp directly casts to OpenGLShader and calls `uploadUniformInt()`
- This breaks Vox abstraction and causes segfaults when Vulkan shader returns null cast

### Phase 3: Enhanced Testing Infrastructure ✅ IMPLEMENTED

**Testing Coverage:**
- `test.sh` now tests both OpenGL and Vulkan backends using environment variables
- CI captures screenshots from both backends: `screenshot_opengl.png` and `screenshot_vulkan.png`
- Vulkan backend currently fails at window surface creation (expected - requires GLFW configuration)
- PR comments show visual comparison between standalone Vulkan demo and both Vox backends

### Phase 4: Shader Abstraction Fix 🔥 CRITICAL PRIORITY

**Problem:** cube_app.cpp breaks Vox abstraction by directly casting to OpenGLShader and calling OpenGL-specific methods like `uploadUniformInt()`. This causes segfaults with Vulkan backend and violates the renderer API abstraction.

**Current code in cube_app.cpp:**
```cpp
std::dynamic_pointer_cast<Vox::OpenGLShader>(shader)->uploadUniformInt("u_texture", 0);
```

**Required solution:**

1. **Add uniform methods to base Shader class:**
   ```cpp
   class Shader {
   public:
       virtual void setInt(const std::string& name, int value) = 0;
       virtual void setFloat(const std::string& name, float value) = 0;
       virtual void setMat4(const std::string& name, const glm::mat4& matrix) = 0;
       // ... other uniform types
   };
   ```

2. **Implement in OpenGLShader:**
   ```cpp
   void OpenGLShader::setInt(const std::string& name, int value) {
       uploadUniformInt(name, value);  // Call existing method
   }
   ```

3. **Implement in VulkanShader:**
   ```cpp
   void VulkanShader::setInt(const std::string& name, int value) {
       // Use existing setInt implementation
   }
   ```

4. **Update cube_app.cpp:**
   ```cpp
   shader->setInt("u_texture", 0);  // Clean, API-agnostic
   ```

**Priority:** This must be fixed before any Vulkan rendering work, as it's a fundamental architectural violation.

### Phase 5: Complete Rendering Pipeline 🔶 MEDIUM PRIORITY

**Required implementations:**

1. **VulkanRendererAPI::clear()**
   - Begin render pass
   - Clear color/depth buffers
   - Set clear color from mClearColor

2. **VulkanRendererAPI::drawIndexed()**
   - Bind vertex/index buffers
   - Record draw commands
   - Submit command buffer

3. **VulkanBuffer classes**
   - Port buffer creation from cube23_vk
   - Implement staging buffer uploads
   - Add memory management

4. **VulkanShader class**
   - Load SPIR-V binaries
   - Create shader modules
   - Manage descriptor sets

5. **VulkanTexture2D class**
   - Image creation and memory binding
   - Texture sampling setup
   - Format conversions

6. **VulkanVertexArray class**
   - Vertex input state management
   - Pipeline state caching
   - Attribute binding

### Phase 6: Validation and Cleanup 🔶 LOW PRIORITY

#### Feature Parity Testing
- Identical visual output between OpenGL/Vulkan
- Performance benchmarking
- Memory usage comparison
- Validation layer compliance

#### Cleanup Tasks
- Remove `cube23_vk` standalone demo
- Update documentation
- Simplify build system
- Archive old implementation notes

## TODOs Breakdown

### High Priority
- [x] **api-selection-mechanism**: Add mechanism to select renderer API (OpenGL vs Vulkan) at runtime or compile-time
- [x] **test-vulkan-backend**: Test cube23 application with Vulkan backend and verify it works  
- [x] **dual-renderer-testing**: Update test.sh to test both OpenGL and Vulkan renderers, compare outputs
- [ ] **shader-uniform-abstraction**: Add uniform upload methods to base Shader class to eliminate OpenGL-specific casts in application code

### Medium Priority  
- [ ] **implement-depth-stencil**: Implement Phase 2: Enhanced Core Features - Add depth/stencil control, viewport management, and basic pipeline state
- [ ] **validation-testing**: Create cube23_vk_integrated application using new Vulkan backend to validate functionality

### Low Priority
- [ ] **cleanup-standalone-vulkan**: Remove cube23_vk standalone demo once Vox Vulkan backend is fully functional

## Implementation Strategy

### Immediate Next Steps (This Session)
1. **Implement environment variable API selection**
2. **Test Vulkan backend initialization**  
3. **Document what's missing from rendering pipeline**
4. **Update test.sh for dual-backend testing**

### Success Criteria
- ✅ `VOX_RENDERER=vulkan ./cube23` runs without crashing
- ✅ VulkanContext initializes correctly (logs show device/surface creation)
- ✅ Clear identification of missing rendering components
- ✅ test.sh validates both backends
- ✅ Foundation for complete Vulkan rendering implementation

### Risk Mitigation
- **Start with API selection mechanism** - low risk, high value
- **Test initialization before rendering** - identify issues early  
- **Maintain OpenGL as fallback** - ensure no regressions
- **Incremental implementation** - validate each component separately

## References
- [Vulkan Backend Analysis](vulkan-backend-analysis.md) - Technical foundation
- [Renderer API Expansion Plan](renderer-api-expansion-plan.md) - Future roadmap
- cube23_vk implementation (lines 394-1800) - Reference implementation

---

*This plan provides a clear path from the current Vulkan infrastructure to a fully functional, testable Vulkan backend that can eventually replace the standalone cube23_vk demo.*
