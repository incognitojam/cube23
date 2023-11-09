#include "vox/renderer/renderer.h"

namespace Vox {
    Renderer::SceneData *Renderer::sSceneData = new SceneData;

    void Renderer::beginScene(const OrthographicCamera &camera) {
        sSceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
    }

    void Renderer::endScene() {
    }

    void Renderer::submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray) {
        shader->bind();
        shader->uploadUniformMat4("u_ViewProjection", sSceneData->viewProjectionMatrix);

        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }
}
