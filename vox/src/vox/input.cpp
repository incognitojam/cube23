#include "vox/input.h"

#include "vox/application.h"

namespace Vox {
    Input *Input::sInstance = new Input();

    bool Input::isKeyPressedImpl(unsigned int keycode) {
        auto window = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isMouseButtonPressedImpl(unsigned int button) {
        auto window = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
        auto state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> Input::getMousePositionImpl() {
        auto window = static_cast<GLFWwindow *>(Application::get().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {xpos, ypos};
    }

    float Input::getMouseXImpl() {
        auto[x, y] = getMousePositionImpl();
        return (float) x;
    }

    float Input::getMouseYImpl() {
        auto[x, y] = getMousePositionImpl();
        return (float) y;
    }
}
