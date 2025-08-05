#include "vox/application.h"

#include <memory>
#include <chrono>
#include <cstdlib>
#include <iostream>

#include "vox/input.h"
#include "vox/renderer/buffer.h"
#include "vox/renderer/renderer.h"

namespace Vox {
    Application *Application::sInstance = nullptr;

    Application::Application(const std::string &name) {
        if (sInstance != nullptr) {
            throw std::runtime_error("Application already exists!");
        }
        sInstance = this;

        mWindow = std::unique_ptr<Window>(Window::create(name));
        mWindow->setEventCallback(VX_BIND_EVENT_FN(Application::onEvent));

        Renderer::init();
    }

    void Application::onEvent(Event &e) {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(VX_BIND_EVENT_FN(Application::onWindowClose));
    }

    void Application::run() {
        auto startTime = std::chrono::high_resolution_clock::now();
        const auto testDuration = std::chrono::seconds(5); // Run for 5 seconds in test mode

        while (mRunning) {
            mWindow->onUpdate();

            const auto time = static_cast<float>(glfwGetTime());
            const Timestep timestep = time - mLastFrameTime;
            mLastFrameTime = time;
            onUpdate(timestep);

            // Auto-close after testDuration if TEST_MODE environment variable is set
            if (std::getenv("TEST_MODE")) {
                auto currentTime = std::chrono::high_resolution_clock::now();
                if (currentTime - startTime >= testDuration) {
                    std::cout << "Test mode: auto-closing after " << testDuration.count() << " seconds" << std::endl;
                    mRunning = false;
                }
            }
        }
    }

    bool Application::onWindowClose(WindowCloseEvent &e) {
        mRunning = false;
        return true;
    }
}
