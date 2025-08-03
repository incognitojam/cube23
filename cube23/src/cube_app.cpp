#include <Vox.h>

#include <glm/gtc/matrix_transform.hpp>

#include "platform/opengl/shader.h"

class Cube final : public Vox::Application {
public:
    Cube() : mCamera(-1.6f, 1.6f, -0.9f, 0.9f), mCameraPosition(0.0f), mTime(0.0f) {
        mVertexArray.reset(Vox::VertexArray::create());

        float vertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        std::shared_ptr<Vox::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Vox::VertexBuffer::create(vertices, sizeof(vertices)));
        const Vox::BufferLayout layout = {
            { Vox::ShaderDataType::Float3, "a_position" },
            { Vox::ShaderDataType::Float2, "a_texCoord" }
        };
        vertexBuffer->setLayout(layout);
        mVertexArray->addVertexBuffer(vertexBuffer);

        uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Vox::IndexBuffer> indexBuffer;
        indexBuffer.reset(Vox::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
        mVertexArray->setIndexBuffer(indexBuffer);

        const auto shader = mShaderLibrary.load("shaders/texture.glsl");

        mTexture = Vox::Texture2D::create("textures/texture.jpg");
        mYingaTexture = Vox::Texture2D::create("textures/yinga.png");

        shader->bind();
        std::dynamic_pointer_cast<Vox::OpenGLShader>(shader)->uploadUniformInt("u_texture", 0);
    }

    ~Cube() {}

    void onUpdate(const Vox::Timestep ts) override {
        mTime += ts;
        if (Vox::Input::isKeyPressed(VX_KEY_LEFT))
            mCameraPosition.x -= mCameraMoveSpeed * ts;
        else if (Vox::Input::isKeyPressed(VX_KEY_RIGHT))
            mCameraPosition.x += mCameraMoveSpeed * ts;

        if (Vox::Input::isKeyPressed(VX_KEY_UP))
            mCameraPosition.y += mCameraMoveSpeed * ts;
        else if (Vox::Input::isKeyPressed(VX_KEY_DOWN))
            mCameraPosition.y -= mCameraMoveSpeed * ts;

        if (Vox::Input::isKeyPressed(VX_KEY_A))
            mCameraRotation += mCameraRotationSpeed * ts;
        else if (Vox::Input::isKeyPressed(VX_KEY_D))
            mCameraRotation -= mCameraRotationSpeed * ts;

        Vox::RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        Vox::RenderCommand::clear();

        mCamera.setPosition(mCameraPosition);
        mCamera.setRotation(mCameraRotation);

        Vox::Renderer::beginScene(mCamera);

        const auto shader = mShaderLibrary.get("texture");

        mTexture->bind(0);

        for (int i = 0; i < 150; i++) {
            float angle = i * 0.3f + mTime * 2.0f;
            float radius = 0.3f + 0.2f * sin(mTime * 1.5f + i * 0.1f);
            float x = cos(angle) * radius;
            float y = sin(angle) * radius;

            float scaleValue = 0.05f + 0.03f * sin(mTime * 3.0f + i * 0.2f);
            float rotation = mTime * 90.0f + i * 10.0f;

            glm::vec3 pos(x, y, 0.0f);
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                                glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                                glm::scale(glm::mat4(1.0f), glm::vec3(scaleValue));

            if (i % 3 == 0) mTexture->bind(0);
            else mYingaTexture->bind(0);

            Vox::Renderer::submit(shader, mVertexArray, transform);
        }

        mYingaTexture->bind(0);
        Vox::Renderer::submit(shader, mVertexArray);

        Vox::Renderer::endScene();
    }

private:
    Vox::ShaderLibrary mShaderLibrary;
    std::shared_ptr<Vox::VertexArray> mVertexArray;

    std::shared_ptr<Vox::Texture2D> mTexture, mYingaTexture;

    Vox::OrthographicCamera mCamera;
    glm::vec3 mCameraPosition;
    float mCameraMoveSpeed = 5.0f;
    float mCameraRotation = 0.0f;
    float mCameraRotationSpeed = 180.0f;
    float mTime;
};

Vox::Application *Vox::create_application() {
    return new Cube();
}
