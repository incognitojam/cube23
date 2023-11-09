#include "vox/application.h"

#include <iostream>

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

        glGenBuffers(1, &mVertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

        float vertices[3 * 3] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
        };

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

        glGenBuffers(1, &mIndexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);

        unsigned int indices[3] = {0, 1, 2};
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }

    void Application::onEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(VX_BIND_EVENT_FN(Application::onWindowClose));
    }

    void Application::run() {
        while (mRunning) {
            glClearColor(0.1f, 0.1f, 0.1f, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            glBindVertexArray(mVertexArray);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
