# RendererAPI Expansion Plan

*Plan created on 2025-08-02*

## Current State

The Vox RendererAPI (`vox/src/vox/renderer/renderer_api.h`) is currently minimal with only basic rendering functionality:

```cpp
enum class API { None = 0, OpenGL = 1 };

virtual void init() = 0;
virtual void setClearColor(const glm::vec4 &color) = 0;
virtual void clear() = 0;
virtual void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) = 0;
```

## Expansion Goals

Transform the RendererAPI into a comprehensive graphics abstraction that supports modern rendering features while maintaining API agnosticism.

## Phase 1: Core Vulkan Integration

### Immediate Changes
1. **Expand API enum**:
   ```cpp
   enum class API { 
       None = 0, 
       OpenGL = 1, 
       Vulkan = 2,
       // Future: DirectX11 = 3, DirectX12 = 4, Metal = 5, WebGL = 6
   };
   ```

2. **Create Vulkan platform implementation**:
   - `vox/src/platform/vulkan/renderer_api.h/cpp`
   - Port initialization logic from `cube23/src/main.cpp:394-577`
   - Port swapchain management from `cube23/src/main.cpp:579-630`

3. **Update factory methods** in existing renderer components to support Vulkan backend

### Priority Features for Phase 1
- Basic rendering (already working in cube23_vk)
- Swapchain management with window resize support
- Command buffer recording and submission
- Basic synchronization (semaphores, fences)

**Estimated Timeline**: 1-2 weeks

## Phase 2: Enhanced Core Features

### Depth and Stencil Control
```cpp
virtual void setDepthTest(bool enabled) = 0;
virtual void setDepthFunc(CompareFunc func) = 0;
virtual void clearDepth(float depth = 1.0f) = 0;
virtual void setStencilTest(bool enabled) = 0;
```

### Viewport and Scissor Management
```cpp
virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
virtual void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
```

### Basic Pipeline State
```cpp
virtual void setCullMode(CullMode mode) = 0;
virtual void setBlendMode(BlendMode mode) = 0;
virtual void setWireframe(bool enabled) = 0;
virtual void setPrimitiveTopology(PrimitiveType type) = 0;
```

**Reference Implementation**: cube23_vk lines 790-833 (pipeline state configuration)

**Estimated Timeline**: 1 week

## Phase 3: Advanced Rendering Features

### Multi-Pass Rendering
```cpp
virtual void beginRenderPass(const RenderPassDesc& desc) = 0;
virtual void endRenderPass() = 0;
virtual void setRenderTarget(std::shared_ptr<RenderTarget> target) = 0;
virtual void setRenderTargets(const std::vector<std::shared_ptr<RenderTarget>>& targets) = 0;
```

### Enhanced Drawing Commands
```cpp
virtual void drawArrays(PrimitiveType type, uint32_t first, uint32_t count) = 0;
virtual void drawInstanced(const std::shared_ptr<VertexArray>& va, uint32_t instanceCount) = 0;
virtual void drawIndirect(const std::shared_ptr<Buffer>& cmdBuffer) = 0;
virtual void multiDrawIndexed(const std::vector<DrawIndexedCommand>& commands) = 0;
```

### Compute Shader Support
```cpp
virtual void dispatch(uint32_t groupX, uint32_t groupY, uint32_t groupZ) = 0;
virtual void memoryBarrier(BarrierType type) = 0;
virtual void dispatchIndirect(const std::shared_ptr<Buffer>& cmdBuffer) = 0;
```

**Reference Implementation**: Vulkan compute concepts can be added using cube23_vk's command buffer system

**Estimated Timeline**: 2-3 weeks

## Phase 4: Resource Management and Utilities

### Advanced Buffer Operations
```cpp
virtual void updateBuffer(std::shared_ptr<Buffer> buffer, const void* data, size_t size, size_t offset = 0) = 0;
virtual void copyBuffer(std::shared_ptr<Buffer> src, std::shared_ptr<Buffer> dst, size_t size) = 0;
virtual void clearBuffer(std::shared_ptr<Buffer> buffer, uint32_t clearValue) = 0;
```

### Texture Operations
```cpp
virtual void generateMipmaps(std::shared_ptr<Texture> texture) = 0;
virtual void copyTexture(std::shared_ptr<Texture> src, std::shared_ptr<Texture> dst) = 0;
virtual void clearTexture(std::shared_ptr<Texture> texture, const glm::vec4& clearColor) = 0;
```

### Performance and Debug Features
```cpp
// Performance queries
virtual std::shared_ptr<Query> createQuery(QueryType type) = 0;
virtual void beginQuery(std::shared_ptr<Query> query) = 0;
virtual void endQuery(std::shared_ptr<Query> query) = 0;
virtual uint64_t getQueryResult(std::shared_ptr<Query> query) = 0;

// Debug markers (for RenderDoc, PIX, etc.)
virtual void pushDebugGroup(const std::string& name) = 0;
virtual void popDebugGroup() = 0;
virtual void insertDebugMarker(const std::string& name) = 0;

// GPU timestamps
virtual std::shared_ptr<TimestampQuery> createTimestampQuery() = 0;
virtual void writeTimestamp(std::shared_ptr<TimestampQuery> query) = 0;
```

**Estimated Timeline**: 2-3 weeks

## Implementation Strategy

### Code Organization
1. **Maintain existing OpenGL backend** - no breaking changes
2. **Add Vulkan platform directory**: `vox/src/platform/vulkan/`
3. **Extend factory pattern** in existing components (Buffer, Shader, Texture, etc.)
4. **Update CMakeLists.txt** to conditionally compile Vulkan support

### Vulkan-Specific Considerations
1. **Command Buffer Management**: Vulkan requires explicit command recording
   - Implement command buffer pooling system
   - Handle primary vs secondary command buffers
   - Manage command buffer lifecycle

2. **Memory Management**: Port from cube23_vk implementation
   - Buffer/image allocation functions (lines 908-936, 1032-1066)
   - Memory type selection (lines 1309-1322)
   - Staging buffer optimization (lines 1202-1232)

3. **Synchronization**: Essential for Vulkan correctness
   - Frame synchronization (lines 1479-1498)
   - Pipeline barriers for resource transitions
   - Queue submission ordering

4. **Pipeline State Objects**: Vulkan requires pre-compiled pipelines
   - Cache pipeline objects based on state combinations
   - Handle dynamic state vs static state
   - Implement pipeline derivatives for performance

### Testing Strategy
1. **Maintain existing cube23 application** as OpenGL reference
2. **Create cube23_vk_integrated** using new Vulkan backend
3. **Compare rendering output** between OpenGL and Vulkan versions
4. **Performance benchmarking** between APIs

## Benefits of This Expansion

### For Developers
- **API Agnostic Code**: Write once, run on multiple graphics APIs
- **Modern Features**: Access to compute shaders, advanced blending, etc.
- **Better Performance**: Vulkan's explicit control and multi-threading support
- **Future Proof**: Easy to add new APIs (DirectX12, Metal, WebGPU)

### For Engine Architecture
- **Clean Abstraction**: High-level rendering concepts independent of API
- **Maintainable**: Single codebase supporting multiple backends
- **Extensible**: Easy to add new rendering features uniformly across APIs
- **Debuggable**: Consistent debug interfaces across platforms

## Risk Mitigation

### Potential Challenges
1. **API Differences**: Some Vulkan concepts don't map cleanly to OpenGL
2. **Performance Overhead**: Abstraction layer might reduce Vulkan performance benefits
3. **Complexity**: Managing multiple backends increases maintenance burden

### Mitigation Strategies
1. **Progressive Enhancement**: Start with common subset, add API-specific features gradually
2. **Performance Profiles**: Allow low-level access for performance-critical code
3. **Automated Testing**: Comprehensive test suite to catch regressions
4. **Documentation**: Clear guidelines on when to use which API

## Success Metrics

1. **Functional Parity**: cube23 application works identically with both backends
2. **Performance**: Vulkan backend shows measurable performance improvements
3. **Developer Experience**: New applications can easily switch between APIs
4. **Code Quality**: No significant increase in complexity for basic use cases

## Next Steps

1. **Review and approve this plan** with stakeholders
2. **Set up development branch** for Vulkan integration
3. **Begin Phase 1 implementation** starting with API enum expansion
4. **Create integration test application** to validate progress

---

*This plan leverages the existing comprehensive Vulkan implementation in cube23_vk as a reference, significantly reducing implementation risk and development time.*