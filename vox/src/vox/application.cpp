#include "vox/application.h"

#include <iostream>
#include <memory>

#include "vox/input.h"
#include "vox/key_codes.h"
#include "vox/renderer/buffer.h"
#include "vox/renderer/renderer.h"

namespace Vox {
    Application *Application::sInstance = nullptr;

    Application::Application()
        : mCamera(-1.6f, 1.6f, -0.9f, 0.9f) {
        if (sInstance != nullptr) {
            throw std::runtime_error("Application already exists!");
        }
        sInstance = this;

        mWindow = std::unique_ptr<Window>(Window::create());
        mWindow->setEventCallback(VX_BIND_EVENT_FN(Application::onEvent));

        mVertexArray.reset(VertexArray::create());

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };

        std::shared_ptr<VertexBuffer> vertexBuffer;
        vertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float4, "a_Color" }
        };
        vertexBuffer->setLayout(layout);
        mVertexArray->addVertexBuffer(vertexBuffer);

        uint32_t indices[3] = {0, 1, 2};
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));
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
        mShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
    }

    void Application::onEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(VX_BIND_EVENT_FN(Application::onWindowClose));
    }

    void Application::run() {
        static float rotation = 0.0f;
        while (mRunning) {
            RenderCommand::setClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
            RenderCommand::clear();

            rotation += 0.5f;
            mCamera.setPosition({ 0.5f, 0.5f, 0.0f });
            mCamera.setRotation(rotation);

            Renderer::beginScene(mCamera);

            Renderer::submit(mShader, mVertexArray);

            Renderer::endScene();

            mWindow->onUpdate();

            if (Input::isKeyPressed(VX_KEY_TAB))
                std::cout << "Tab key is pressed (poll)!" << std::endl;
        }
    }

    bool Application::onWindowClose(WindowCloseEvent &e) {
        mRunning = false;
        return true;
    }
}
