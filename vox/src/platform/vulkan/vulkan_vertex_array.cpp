#include "vulkan_vertex_array.h"

#include <stdexcept>

namespace Vox {
    VulkanVertexArray::VulkanVertexArray() {
        // TODO: Initialize Vulkan vertex input state
    }

    VulkanVertexArray::~VulkanVertexArray() {
        // TODO: Cleanup Vulkan vertex array resources
    }

    void VulkanVertexArray::bind() const {
        // TODO: Set Vulkan pipeline vertex input state
    }

    void VulkanVertexArray::unbind() const {
        // TODO: Unset Vulkan vertex input state
    }

    void VulkanVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer> &vertexBuffer) {
        // Simple assert check without logging macros
        if (vertexBuffer->getLayout().getElements().empty()) {
            throw std::runtime_error("Vertex Buffer has no layout!");
        }

        mVertexBuffers.push_back(vertexBuffer);
        
        // TODO: Update Vulkan vertex input attribute descriptions
    }

    void VulkanVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer) {
        mIndexBuffer = indexBuffer;
        
        // TODO: Update Vulkan index buffer binding
    }
}
