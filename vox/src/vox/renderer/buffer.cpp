#include "vox/renderer/buffer.h"

#include <stdexcept>

#include "vox/renderer/renderer.h"

#include "platform/opengl/buffer.h"

namespace Vox {
    VertexBuffer *VertexBuffer::create(float *vertices, uint32_t size) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return new OpenGLVertexBuffer(vertices, size);
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
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }

    std::shared_ptr<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLUniformBuffer>(size, binding);
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }
}
