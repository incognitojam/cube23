#include <Vox.h>

class Cube : public Vox::Application {
public:
    Cube() {}

    ~Cube() {}
};

Vox::Application *Vox::create_application() {
    return new Cube();
}
