#include "vulkan_renderer_api.h"
#include "vulkan_context.h"
#include "vox/application.h"

namespace Vox {
    void VulkanRendererAPI::init() {
        // Context is already initialized by the Window class
        // Just verify we have a valid Vulkan context
        VulkanContext* context = getContext();
        if (!context) {
            throw std::runtime_error("Failed to get Vulkan context!");
        }
    }

    void VulkanRendererAPI::setClearColor(const glm::vec4 &color) {
        mClearColor = color;
    }

    void VulkanRendererAPI::clear() {
        // TODO: Implement Vulkan clear operation using the context
        VulkanContext* context = getContext();
        // Will implement actual clearing later
    }

    void VulkanRendererAPI::drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) {
        // TODO: Implement Vulkan indexed drawing using the context
        VulkanContext* context = getContext();
        // Will implement actual drawing later
    }

    VulkanContext* VulkanRendererAPI::getContext() {
        auto& window = Application::get().getWindow();
        return dynamic_cast<VulkanContext*>(window.getGraphicsContext());
    }
}
