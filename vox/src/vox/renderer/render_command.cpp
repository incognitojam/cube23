#include "vox/renderer/render_command.h"

#include "platform/opengl/renderer_api.h"

namespace Vox {
    RendererAPI *RenderCommand::sRendererAPI = []() {
        switch (RendererAPI::getAPI()) {
            case RendererAPI::API::None:
                throw std::runtime_error("RendererAPI::None is currently not supported!");
            case RendererAPI::API::OpenGL:
                return static_cast<RendererAPI*>(new OpenGLRendererAPI());
            default:
                throw std::runtime_error("Unknown RendererAPI!");
        }
    }();
}
