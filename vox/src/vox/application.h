#pragma once

#include <memory>

#include "vox/window.h"

namespace Vox {
    class Application {
    public:
        Application();
        virtual ~Application();

        virtual void run();
    private:
        std::unique_ptr<Window> mWindow;
        bool mRunning = true;
    };

    Application *create_application();
}
