#pragma once

#include "vox/renderer/render_command.h"

namespace Vox {
    class Renderer {
    public:
        static void beginScene();
        static void endScene();

        static void submit(const std::shared_ptr<VertexArray> &vertexArray);

        static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }
    };
}
