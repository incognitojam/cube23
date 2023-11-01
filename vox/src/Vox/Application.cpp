#include "vox/application.h"
#include "vox/events/application_event.h"

#include <iostream>

namespace Vox {

    Application::Application() {}

    Application::~Application() {}

    void Application::run() {
        WindowResizeEvent e(1280, 720);
        if (e.isInCategory(EventCategoryApplication)) {
            std::cout << e << std::endl;
        }
        if (e.isInCategory(EventCategoryInput)) {
            std::cout << e << std::endl;
        }

        while (true);
    }

}
