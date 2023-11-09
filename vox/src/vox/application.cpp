#include "vox/application.h"

#include <iostream>
#include <memory>

#include <glad/glad.h>

#include "vox/input.h"
#include "vox/key_codes.h"

namespace Vox {
    Application *Application::sInstance = nullptr;

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:
            case ShaderDataType::Float2:
            case ShaderDataType::Float3:
            case ShaderDataType::Float4:
            case ShaderDataType::Mat3:
            case ShaderDataType::Mat4: return GL_FLOAT;
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
            default:
                throw std::runtime_error("Unknown ShaderDataType!");
        }
    }

    Application::Application() {
        if (sInstance != nullptr) {
            throw std::runtime_error("Application already exists!");
        }
        sInstance = this;

        mWindow = std::unique_ptr<Window>(Window::create());
        mWindow->setEventCallback(VX_BIND_EVENT_FN(Application::onEvent));

        glGenVertexArrays(1, &mVertexArray);
        glBindVertexArray(mVertexArray);

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
             0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
             0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
        };
        mVertexBuffer.reset(VertexBuffer::create(vertices, sizeof(vertices)));
        {
            BufferLayout layout = {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color" }
            };
            mVertexBuffer->setLayout(layout);
        }

        uint32_t index = 0;
        const auto &layout = mVertexBuffer->getLayout();
        for (const auto &element : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(index,
                                  element.getComponentCount(),
                                  ShaderDataTypeToOpenGLBaseType(element.type),
                                  element.normalized ? GL_TRUE : GL_FALSE,
                                  layout.getStride(),
                                  (const void *)element.offset);
            index++;
        }

        uint32_t indices[3] = {0, 1, 2};
        mIndexBuffer.reset(IndexBuffer::create(indices, sizeof(indices) / sizeof(uint32_t)));

        std::string vertexSrc = R"(#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

out vec3 v_Position;
out vec4 v_Color;

void main() {
    v_Position = a_Position;
    v_Color = a_Color;
    gl_Position = vec4(a_Position, 1.0);
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
        while (mRunning) {
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            mShader->bind();
            glBindVertexArray(mVertexArray);
            glDrawElements(GL_TRIANGLES, mIndexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);

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
