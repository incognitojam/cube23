#include "vox/renderer/vertex_array.h"

#include "vox/renderer/renderer.h"

#include "platform/opengl/vertex_array.h"
#include "platform/vulkan/vulkan_vertex_array.h"

namespace Vox {
    VertexArray* VertexArray::create() {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return new OpenGLVertexArray();
            case RendererAPI::API::Vulkan:
                return new VulkanVertexArray();
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }
}
