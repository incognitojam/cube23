#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan_context.h"
#include "vox/renderer/graphics_context.h"

namespace Vox {
    GraphicsContext* createVulkanContext(GLFWwindow* window) {
        return new VulkanContext(window);
    }
}
