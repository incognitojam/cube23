#include "vox/window.h"

#include <stdexcept>

namespace Vox {
    Window *Window::create(const std::string &title, int width, int height) {
        static bool s_GLFWInitialized = false;

        if (!s_GLFWInitialized) {
            // TODO: glfwTerminate on application close
            if (glfwInit() != GLFW_TRUE) {
                throw std::runtime_error("Failed to initialize GLFW!");
            }
            s_GLFWInitialized = true;
        }

        return new Window(title, width, height);
    }

    Window::Window(const std::string &title, int width, int height) : mTitle(title), mWidth(width), mHeight(height) {
        mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        if (mWindow == nullptr) {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window!");
        }

        glfwMakeContextCurrent(mWindow);
        glfwSetWindowUserPointer(mWindow, this);
        setVSync(true);
    }

    Window::~Window() {
        glfwDestroyWindow(mWindow);
    }

    void Window::onUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(mWindow);
    }

    void Window::setVSync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
        mVSync = enabled;
    }

    bool Window::isVSync() const {
        return mVSync;
    }
}
