#include "vox/renderer/renderer.h"

#include "platform/opengl/shader.h"

namespace Vox {
    Renderer::SceneData *Renderer::sSceneData = new SceneData;
    std::shared_ptr<UniformBuffer> Renderer::sCameraUniformBuffer = nullptr;
    std::shared_ptr<UniformBuffer> Renderer::sObjectUniformBuffer = nullptr;

    void Renderer::init() {
        RenderCommand::init();
        
        sCameraUniformBuffer = UniformBuffer::create(sizeof(glm::mat4), 0);
        sObjectUniformBuffer = UniformBuffer::create(sizeof(glm::mat4), 1);
    }

    void Renderer::beginScene(const OrthographicCamera &camera) {
        sSceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
        sCameraUniformBuffer->setData(&sSceneData->viewProjectionMatrix, sizeof(glm::mat4));
    }

    void Renderer::endScene() {
    }

    void Renderer::submit(const std::shared_ptr<Shader> &shader, const std::shared_ptr<VertexArray> &vertexArray,
                          const glm::mat4 &transform) {
        shader->bind();
        
        sObjectUniformBuffer->setData(&transform, sizeof(glm::mat4));

        vertexArray->bind();
        RenderCommand::drawIndexed(vertexArray);
    }
}
