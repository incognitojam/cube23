#include "vox/application.h"

#include <iostream>
#include <memory>

#include <glad/glad.h>

#include "vox/input.h"
#include "vox/key_codes.h"

namespace Vox {
    Application *Application::sInstance = nullptr;

    Application::Application() {
        if (sInstance != nullptr) {
            throw std::runtime_error("Application already exists!");
        }
        sInstance = this;

        mWindow = std::unique_ptr<Window>(Window::create());
        mWindow->setEventCallback(VX_BIND_EVENT_FN(Application::onEvent));

        glGenVertexArrays(1, &mVertexArray);
        glBindVertexArray(mVertexArray);

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        mVertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        uint32_t indices[3] = {0, 1, 2};
        mIndexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(#version 330 core

layout (location = 0) in vec3 a_Position;

out vec3 v_Position;

void main() {
    v_Position = a_Position;
    gl_Position = vec4(a_Position, 1.0);
}
)";
        std::string fragmentSrc = R"(#version 330 core

layout (location = 0) out vec4 color;

in vec3 v_Position;

void main() {
    color = vec4(v_Position * 0.5 + 0.5, 1.0);
}
)";
        mShader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
    }

    void Application::onEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(VX_BIND_EVENT_FN(Application::onWindowClose));
    }

    void Application::run() {
        while (mRunning) {
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            mShader->bind();
            glBindVertexArray(mVertexArray);
            glDrawElements(GL_TRIANGLES, mIndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

            mWindow->onUpdate();

            if (Vox::Input::isKeyPressed(VX_KEY_TAB))
                std::cout << "Tab key is pressed (poll)!" << std::endl;
        }
    }

    bool Application::onWindowClose(WindowCloseEvent &e) {
        mRunning = false;
        return true;
    }
}
