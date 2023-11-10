#include <vox.h>

class Cube final : public Vox::Application {
public:
    Cube() : mCamera(-1.6f, 1.6f, -0.9f, 0.9f), mCameraPosition(0.0f) {
        mVertexArray.reset(Vox::VertexArray::create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<Vox::VertexBuffer> vertexBuffer;
        vertexBuffer.reset(Vox::VertexBuffer::create(vertices, sizeof(vertices)));
        Vox::BufferLayout layout = {
            { Vox::ShaderDataType::Float3, "a_Position" },
            { Vox::ShaderDataType::Float4, "a_Color" }
        };
        vertexBuffer->setLayout(layout);
        mVertexArray->addVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<Vox::IndexBuffer> indexBuffer;
        indexBuffer.reset(Vox::IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
        mVertexArray->setIndexBuffer(indexBuffer);

        std::string vertexSrc = R"(#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec3 v_Position;
out vec4 v_Color;

void main() {
    v_Position = a_Position;
    v_Color = a_Color;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}
)";
        std::string fragmentSrc = R"(#version 330 core

layout (location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;

void main() {
    color = vec4(v_Position * 0.5 + 0.5, 1.0);
    color = v_Color;
}
)";
        mShader.reset(Vox::Shader::create(vertexSrc, fragmentSrc));
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
        Vox::Renderer::submit(mShader, mVertexArray);
        Vox::Renderer::endScene();
    }

private:
    std::shared_ptr<Vox::Shader> mShader;
    std::shared_ptr<Vox::VertexArray> mVertexArray;

    Vox::OrthographicCamera mCamera;
    glm::vec3 mCameraPosition;
    float mCameraMoveSpeed = 5.0f;
    float mCameraRotation = 0.0f;
    float mCameraRotationSpeed = 180.0f;
};

Vox::Application *Vox::create_application() {
    return new Cube();
}
