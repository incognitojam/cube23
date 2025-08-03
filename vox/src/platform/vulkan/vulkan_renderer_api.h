#pragma once

#include "vox/renderer/renderer_api.h"

namespace Vox {
    class VulkanRendererAPI : public RendererAPI {
    public:
        void init() override;

        void setClearColor(const glm::vec4 &color) override;
        void clear() override;

        void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray) override;

    private:
        glm::vec4 mClearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    };
}
