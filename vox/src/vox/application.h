#pragma once

#include <memory>

#include "vox/events/application_event.h"
#include "vox/events/event.h"
#include "vox/renderer/shader.h"
#include "vox/window.h"

namespace Vox {
    class Application {
    public:
        Application();
        virtual ~Application() = default;

        virtual void run();

        void onEvent(Event &);

        inline Window &getWindow() { return *mWindow; }

        inline static Application &get() { return *sInstance; }

    private:
        bool onWindowClose(WindowCloseEvent &);

        std::unique_ptr<Window> mWindow;
        bool mRunning = true;

        unsigned int mVertexArray, mVertexBuffer, mIndexBuffer;
        std::unique_ptr<Shader> mShader;

    private:
        static Application *sInstance;
    };

    Application *create_application();
}
