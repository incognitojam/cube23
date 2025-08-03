#include "vox/renderer/shader.h"

#include "vox/renderer/renderer.h"

#include "platform/opengl/shader.h"
#include "platform/vulkan/vulkan_shader.h"

namespace Vox {
    std::shared_ptr<Shader> Shader::create(const std::string &filepath) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(filepath);
            case RendererAPI::API::Vulkan:
                return std::make_shared<VulkanShader>(filepath);
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }

    std::shared_ptr<Shader> Shader::create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
            case RendererAPI::API::Vulkan:
                return std::make_shared<VulkanShader>(name, vertexSrc, fragmentSrc);
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }

    void ShaderLibrary::add(const std::string &name, const std::shared_ptr<Shader> &shader) {
        if (exists(name)) {
            throw std::runtime_error("Shader already exists!");
        }
        mShaders[name] = shader;
    }

    void ShaderLibrary::add(const std::shared_ptr<Shader> &shader) {
        auto &name = shader->getName();
        add(name, shader);
    }

    std::shared_ptr<Shader> ShaderLibrary::load(const std::string &filepath) {
        auto shader = Shader::create(filepath);
        add(shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::load(const std::string &name, const std::string &filepath) {
        auto shader = Shader::create(filepath);
        add(name, shader);
        return shader;
    }

    std::shared_ptr<Shader> ShaderLibrary::get(const std::string &name) {
        if (!exists(name)) {
            throw std::runtime_error("Shader not found!");
        }
        return mShaders[name];
    }

    bool ShaderLibrary::exists(const std::string &name) const {
        return mShaders.contains(name);
    }
}
