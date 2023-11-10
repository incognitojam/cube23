#include "vox/application.h"

#include <memory>

#include "vox/input.h"
#include "vox/renderer/buffer.h"

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
            mWindow->onUpdate();

            const float time = static_cast<float>(glfwGetTime());
            const Timestep timestep = time - mLastFrameTime;
            mLastFrameTime = time;
            onUpdate(timestep);
        }
    }

    bool Application::onWindowClose(WindowCloseEvent &e) {
        mRunning = false;
        return true;
    }
}
