#pragma once

#include "vox/renderer/renderer_api.h"

namespace Vox {
    class RenderCommand {
    public:
        static void setClearColor(const glm::vec4 &color) {
            sRendererApi->setClearColor(color);
        }

        static void clear() {
            sRendererApi->clear();
        }

        static void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) {
            sRendererApi->drawIndexed(vertexArray);
        }

    private:
        static RendererAPI *sRendererApi;
    };
}
