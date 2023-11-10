#include "vox/renderer/shader.h"

#include "vox/renderer/renderer.h"

#include "platform/opengl/shader.h"

namespace Vox {
    Shader * Shader::create(const std::string &filepath) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(filepath);
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }

    Shader *Shader::create(const std::string &vertexSrc, const std::string &fragmentSrc) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return new OpenGLShader(vertexSrc, fragmentSrc);
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }
}
