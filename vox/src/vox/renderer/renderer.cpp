#include "vox/renderer/renderer.h"

#include "platform/opengl/shader.h"

namespace Vox {
    Renderer::SceneData *Renderer::sSceneData = new SceneData;

    void Renderer::init() {
        RenderCommand::init();
    }

    void Renderer::beginScene(const OrthographicCamera &camera) {
        sSceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::endScene() {
    }

    void Renderer::submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray,
                          const glm::mat4 &transform) {
        shader->bind();
        shader->setMat4("u_viewProjection", sSceneData->viewProjectionMatrix);
        shader->setMat4("u_transform", transform);

        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }
}
