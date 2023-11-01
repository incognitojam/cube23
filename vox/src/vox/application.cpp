#include "vox/application.h"

#include <iostream>

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
    }

    void Application::onEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(VX_BIND_EVENT_FN(Application::onWindowClose));
    }

    void Application::run() {
        while (mRunning) {
            glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
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
