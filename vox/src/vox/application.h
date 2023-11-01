#pragma once

#include <memory>

#include "vox/events/application_event.h"
#include "vox/events/event.h"
#include "vox/window.h"

namespace Vox {
    class Application {
    public:
        Application();
        virtual ~Application() = default;

        virtual void run();

        void onEvent(Event &);
    private:
        bool onWindowClose(WindowCloseEvent &);

        std::unique_ptr<Window> mWindow;
        bool mRunning = true;
    };

    Application *create_application();
}
