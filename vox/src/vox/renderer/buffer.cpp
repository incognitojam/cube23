#include "vox/renderer/buffer.h"

#include <stdexcept>

#include "vox/renderer/renderer.h"

#include "platform/opengl/buffer.h"
#include "platform/vulkan/vulkan_buffer.h"

namespace Vox {
    VertexBuffer *VertexBuffer::create(float *vertices, uint32_t size) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return new OpenGLVertexBuffer(vertices, size);
            case RendererAPI::API::Vulkan:
                return new VulkanVertexBuffer(vertices, size);
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }

    IndexBuffer *IndexBuffer::create(uint32_t *indices, uint32_t count) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return new OpenGLIndexBuffer(indices, count);
            case RendererAPI::API::Vulkan:
                return new VulkanIndexBuffer(indices, count);
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }
}
