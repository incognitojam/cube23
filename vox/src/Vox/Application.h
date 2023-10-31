#pragma once

namespace Vox {

    class Application {
    public:
        Application();

        virtual ~Application();

        void run();
    };

    Application *create_application();

}
