#include "vox/application.h"

#include <iostream>

namespace Vox {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application::Application() {
        mWindow = std::unique_ptr<Window>(Window::create());
        mWindow->setEventCallback(BIND_EVENT_FN(onEvent));
    }

    void Application::onEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
    }

    void Application::run() {
        WindowResizeEvent e(1280, 720);
        if (e.isInCategory(EventCategoryApplication)) {
            std::cout << e << std::endl;
        }
        if (e.isInCategory(EventCategoryInput)) {
            std::cout << e << std::endl;
        }

        while (mRunning) {
            glClearColor(0.5f, 0.0f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            mWindow->onUpdate();
        }
    }

    bool Application::onWindowClose(WindowCloseEvent &e) {
        mRunning = false;
        return true;
    }
}
