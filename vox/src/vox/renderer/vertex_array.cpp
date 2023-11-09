#include "vox/renderer/vertex_array.h"

#include "vox/renderer/renderer.h"

#include "platform/opengl/vertex_array.h"

namespace Vox {
    VertexArray* VertexArray::create() {
        switch (Renderer::getApi()) {
            case RendererAPI::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::OpenGL:
                return new OpenGLVertexArray();
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }
}
