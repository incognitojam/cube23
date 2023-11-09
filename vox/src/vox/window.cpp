#include "vox/window.h"

#include <glad/glad.h>

#include <stdexcept>
#include <iostream>

#include "vox/events/application_event.h"
#include "vox/events/mouse_event.h"
#include "vox/events/key_event.h"

#include "platform/opengl/opengl_context.h"

namespace Vox {
    static void GLFWErrorCallback(int error, const char *description) {
        std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
    }

    Window *Window::create(const std::string &title, int width, int height) {
        static bool s_GLFWInitialized = false;

        if (!s_GLFWInitialized) {
            // TODO: glfwTerminate on application close
            if (glfwInit() != GLFW_TRUE) {
                throw std::runtime_error("Failed to initialize GLFW!");
            }
            glfwSetErrorCallback(GLFWErrorCallback);
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

        mContext = new OpenGLContext(mWindow);
        mContext->init();

        glfwMakeContextCurrent(mWindow);
        int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        if (!status) {
            glfwTerminate();
            throw std::runtime_error("Failed to initialize glad!");
        }
        glfwSetWindowUserPointer(mWindow, this);
        setVSync(true);

        // Setup GLFW callbacks
        glfwSetWindowSizeCallback(mWindow, [](GLFWwindow *window, int width, int height) {
            Window &data = *(Window *) glfwGetWindowUserPointer(window);
            data.mWidth = width;
            data.mHeight = height;

            WindowResizeEvent event(width, height);
            data.mEventCallback(event);
        });

        glfwSetWindowCloseCallback(mWindow, [](GLFWwindow *window) {
            Window &data = *(Window *) glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.mEventCallback(event);
        });

        glfwSetKeyCallback(mWindow, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            Window &data = *(Window *) glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressedEvent event(key, 0);
                    data.mEventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    KeyReleasedEvent event(key);
                    data.mEventCallback(event);
                    break;
                }
                case GLFW_REPEAT: {
                    KeyPressedEvent event(key, 1);
                    data.mEventCallback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetCharCallback(mWindow, [](GLFWwindow *window, unsigned int keycode) {
            Window &data = *(Window *) glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.mEventCallback(event);
        });

        glfwSetMouseButtonCallback(mWindow, [](GLFWwindow *window, int button, int action, int mods) {
            Window &data = *(Window *) glfwGetWindowUserPointer(window);

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressedEvent event(button);
                    data.mEventCallback(event);
                    break;
                }
                case GLFW_RELEASE: {
                    MouseButtonReleasedEvent event(button);
                    data.mEventCallback(event);
                    break;
                }
                default:
                    break;
            }
        });

        glfwSetScrollCallback(mWindow, [](GLFWwindow *window, double xOffset, double yOffset) {
            Window &data = *(Window *) glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float) xOffset, (float) yOffset);
            data.mEventCallback(event);
        });

        glfwSetCursorPosCallback(mWindow, [](GLFWwindow *window, double xPos, double yPos) {
            Window &data = *(Window *) glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float) xPos, (float) yPos);
            data.mEventCallback(event);
        });
    }

    Window::~Window() {
        glfwDestroyWindow(mWindow);
    }

    void Window::onUpdate() {
        glfwPollEvents();
        mContext->swapBuffers();
    }

    void Window::setVSync(bool enabled) {
        glfwSwapInterval(enabled ? 1 : 0);
        mVSync = enabled;
    }

    bool Window::isVSync() const {
        return mVSync;
    }
}
