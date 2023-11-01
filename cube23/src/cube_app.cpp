#include <vox.h>

#include <iostream>

class Cube : public Vox::Application {
public:
    Cube() {}

    ~Cube() {}
};

Vox::Application *Vox::create_application() {
    return new Cube();
}
