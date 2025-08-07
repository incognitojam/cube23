#pragma once

#include "vox/renderer/orthographic_camera.h"
#include "vox/renderer/render_command.h"
#include "vox/renderer/shader.h"
#include "vox/renderer/buffer.h"

namespace Vox {
    class Renderer {
    public:
        static void init();

        static void beginScene(const OrthographicCamera &camera);
        static void endScene();

        static void submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray,
                           const glm::mat4 &transform = glm::mat4(1.0f));

        static RendererAPI::API getAPI() { return RendererAPI::getAPI(); }

    private:
        struct SceneData {
            glm::mat4 viewProjectionMatrix;
        };

        static SceneData *sSceneData;
        static std::shared_ptr<UniformBuffer> sCameraUniformBuffer;
        static std::shared_ptr<UniformBuffer> sObjectUniformBuffer;
    };
}
