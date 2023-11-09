#include "vox/renderer/render_command.h"

#include "platform/opengl/renderer_api.h"

namespace Vox {
    RendererAPI *RenderCommand::sRendererApi = new OpenGLRendererAPI();
}
