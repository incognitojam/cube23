#pragma once

#include "vox/renderer/renderer_api.h"

namespace Vox {
    class OpenGLRendererAPI : public RendererAPI {
    public:
        void init() override;

        void setClearColor(const glm::vec4 &color) override;
        void clear() override;

        void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) override;
    };
}
