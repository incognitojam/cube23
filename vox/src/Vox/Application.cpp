#include "vox/application.h"
#include "vox/events/application_event.h"

#include <iostream>

namespace Vox {
    Application::Application() {
        mWindow = std::unique_ptr<Window>(Window::create());
    }

    Application::~Application() {
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
}
