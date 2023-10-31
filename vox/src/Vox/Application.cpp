#include "vox/application.h"

#include <iostream>

namespace Vox {

    Application::Application() {}

    Application::~Application() {}

    void Application::run() {
        std::cout << "Hello, Vox!" << std::endl;
        while (true);
    }

}
