#include "vox/renderer/texture.h"

#include "vox/renderer/renderer.h"

#include "platform/opengl/texture.h"
#include "platform/vulkan/vulkan_texture.h"

namespace Vox {
    std::shared_ptr<Texture2D> Texture2D::create(const std::string &path) {
        switch (Renderer::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is not supported!");
            case RendererAPI::API::OpenGL:
                return std::make_shared<OpenGLTexture2D>(path);
            case RendererAPI::API::Vulkan:
                return std::make_shared<VulkanTexture2D>(path);
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }
}
