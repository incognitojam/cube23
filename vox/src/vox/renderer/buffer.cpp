#include "vox/renderer/buffer.h"

#include <stdexcept>

#include "vox/renderer/renderer.h"

#include "platform/opengl/buffer.h"

namespace Vox {
    VertexBuffer *VertexBuffer::create(float *vertices, uint32_t size) {
        switch (Renderer::getApi()) {
            case RendererAPI::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::OpenGL:
                return new OpenGLVertexBuffer(vertices, size);
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }

    IndexBuffer *IndexBuffer::create(uint32_t *indices, uint32_t count) {
        switch (Renderer::getApi()) {
            case RendererAPI::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::OpenGL:
                return new OpenGLIndexBuffer(indices, count);
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }
}
