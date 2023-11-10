#pragma once

#include "vox/renderer/renderer_api.h"

namespace Vox {
    class RenderCommand {
    public:
        static void init() {
            sRendererAPI->init();
        }

        static void setClearColor(const glm::vec4 &color) {
            sRendererAPI->setClearColor(color);
        }

        static void clear() {
            sRendererAPI->clear();
        }

        static void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) {
            sRendererAPI->drawIndexed(vertexArray);
        }

    private:
        static RendererAPI *sRendererAPI;
    };
}
