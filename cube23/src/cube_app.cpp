#include <vox.h>

#include <glm/gtc/matrix_transform.hpp>

#include "platform/opengl/shader.h"

class Cube final : public Vox::Application {
public:
    Cube() : mCamera(-1.6f, 1.6f, -0.9f, 0.9f), mCameraPosition(0.0f) {
        mVertexArray.reset(Vox::VertexArray::create());

        float vertices[5 * 4] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        std::shared_ptr<Vox::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Vox::VertexBuffer::create(vertices, sizeof(vertices)));
        Vox::BufferLayout layout = {
            { Vox::ShaderDataType::Float3, "a_Position" },
            { Vox::ShaderDataType::Float2, "a_TexCoord" }
        };
        vertexBuffer->setLayout(layout);
        mVertexArray->addVertexBuffer(vertexBuffer);

        uint32_t indices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Vox::IndexBuffer> indexBuffer;
        indexBuffer.reset(Vox::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
        mVertexArray->setIndexBuffer(indexBuffer);

        const std::string vertexSrc = R"(#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;

void main() {
    v_TexCoord = a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}
)";
        const std::string fragmentSrc = R"(#version 330 core

layout (location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;

void main() {
    color = texture(u_Texture, v_TexCoord);
}
)";
        mShader.reset(Vox::Shader::create(vertexSrc, fragmentSrc));

        mTexture = Vox::Texture2D::create("textures/texture.jpg");

        mShader->bind();
        std::dynamic_pointer_cast<Vox::OpenGLShader>(mShader)->uploadUniformInt("u_Texture", 0);
    }

    ~Cube() {}

    void onUpdate(const Vox::Timestep ts) override {
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

        mTexture->bind(0);

        const glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
        for (int y = 0; y < 20; y++) {
            for (int x = 0; x < 20; x++) {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 transform = translate(glm::mat4(1.0f), pos) * scale;
                Vox::Renderer::submit(mShader, mVertexArray, transform);
            }
        }

        Vox::Renderer::endScene();
    }

private:
    std::shared_ptr<Vox::Shader> mShader;
    std::shared_ptr<Vox::VertexArray> mVertexArray;

    std::shared_ptr<Vox::Texture2D> mTexture;

    Vox::OrthographicCamera mCamera;
    glm::vec3 mCameraPosition;
    float mCameraMoveSpeed = 5.0f;
    float mCameraRotation = 0.0f;
    float mCameraRotationSpeed = 180.0f;
};

Vox::Application *Vox::create_application() {
    return new Cube();
}
