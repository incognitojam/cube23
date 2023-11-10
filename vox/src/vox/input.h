#pragma once

#include <utility>

#include "vox/application.h"

namespace Vox {
    class Input {
    protected:
        Input() = default;

    public:
        Input(const Input &) = delete;
        Input &operator=(const Input &) = delete;

        static bool isKeyPressed(const unsigned int keycode) {
            const auto window = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
            const auto state = glfwGetKey(window, keycode);
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

        static bool isMouseButtonPressed(const unsigned int button) {
            const auto window = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
            return glfwGetMouseButton(window, button) == GLFW_PRESS;
        }

        static std::pair<float, float> getMousePosition() {
            const auto window = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            return {xpos, ypos};
        }

        static float getMouseX() {
            auto [x, y] = getMousePosition();
            return x;
        }

        static float getMouseY() {
            auto [x, y] = getMousePosition();
            return y;
        }

    private:
        static Input *sInstance;
    };
}
