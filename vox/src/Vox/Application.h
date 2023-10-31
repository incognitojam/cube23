#pragma once

namespace Vox {

    class Application {
    public:
        Application();

        virtual ~Application();

        virtual void run();
    };

    Application *create_application();

}
