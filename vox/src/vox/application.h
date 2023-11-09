#pragma once

#include <memory>

#include "vox/events/application_event.h"
#include "vox/events/event.h"
#include "vox/renderer/orthographic_camera.h"
#include "vox/renderer/shader.h"
#include "vox/renderer/vertex_array.h"
#include "vox/window.h"

namespace Vox {
    class Application {
    public:
        Application();
        virtual ~Application() = default;

        virtual void run();

        void onEvent(Event &);

        Window &getWindow() const { return *mWindow; }

        static Application &get() { return *sInstance; }

    private:
        bool onWindowClose(WindowCloseEvent &);

        std::unique_ptr<Window> mWindow;
        bool mRunning = true;

        std::shared_ptr<Shader> mShader;
        std::shared_ptr<VertexArray> mVertexArray;

        OrthographicCamera mCamera;

        static Application *sInstance;
    };

    Application *create_application();
}
