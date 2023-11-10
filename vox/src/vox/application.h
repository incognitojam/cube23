#pragma once

#include <memory>

#include "vox/core/timestep.h"
#include "vox/events/application_event.h"
#include "vox/events/event.h"
#include "vox/window.h"

namespace Vox {
    class Application {
    public:
        Application();
        virtual ~Application() = default;

        virtual void run();

        virtual void onUpdate(const Timestep ts) {}

        void onEvent(Event &);

        Window &getWindow() const { return *mWindow; }

        static Application &get() { return *sInstance; }

    private:
        bool onWindowClose(WindowCloseEvent &);

        std::unique_ptr<Window> mWindow;
        bool mRunning = true;
        float mLastFrameTime = 0.0f;

        static Application *sInstance;
    };

    Application *create_application();
}
