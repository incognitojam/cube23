#include "vulkan_buffer.h"

namespace Vox {
    // VulkanVertexBuffer
    VulkanVertexBuffer::VulkanVertexBuffer(float *vertices, uint32_t size) {
        // TODO: Create Vulkan vertex buffer
    }

    VulkanVertexBuffer::~VulkanVertexBuffer() {
        // TODO: Cleanup Vulkan buffer
    }

    void VulkanVertexBuffer::bind() const {
        // TODO: Bind Vulkan vertex buffer
    }

    void VulkanVertexBuffer::unbind() const {
        // TODO: Unbind Vulkan vertex buffer
    }

    // VulkanIndexBuffer
    VulkanIndexBuffer::VulkanIndexBuffer(uint32_t *indices, uint32_t count)
        : mCount(count) {
        // TODO: Create Vulkan index buffer
    }

    VulkanIndexBuffer::~VulkanIndexBuffer() {
        // TODO: Cleanup Vulkan buffer
    }

    void VulkanIndexBuffer::bind() const {
        // TODO: Bind Vulkan index buffer
    }

    void VulkanIndexBuffer::unbind() const {
        // TODO: Unbind Vulkan index buffer
    }
}
