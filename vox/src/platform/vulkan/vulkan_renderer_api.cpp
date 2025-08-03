#include "vulkan_renderer_api.h"

namespace Vox {
    void VulkanRendererAPI::init() {
        // TODO: Initialize Vulkan context
    }

    void VulkanRendererAPI::setClearColor(const glm::vec4 &color) {
        mClearColor = color;
    }

    void VulkanRendererAPI::clear() {
        // TODO: Implement Vulkan clear operation
    }

    void VulkanRendererAPI::drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) {
        // TODO: Implement Vulkan indexed drawing
    }
}
