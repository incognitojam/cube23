#pragma once

extern Vox::Application *Vox::create_application();

int main(int argc, char **argv) {
    auto app = Vox::create_application();
    app->run();
    delete app;
}
