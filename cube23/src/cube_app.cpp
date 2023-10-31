#include <vox.h>

#include <iostream>

class Cube : public Vox::Application {
public:
    Cube() {}

    ~Cube() {}

    void run() override {
        std::cout << "Hello, Cube!" << std::endl;
        while (true);
    }
};

Vox::Application *Vox::create_application() {
    return new Cube();
}
